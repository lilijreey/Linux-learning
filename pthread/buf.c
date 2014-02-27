/**
 * @file     buf.c
 * 多个产生者一个消费着， worker不停给buf 里面写log， log 线程打印这些log，如果buf满了，
 * worker会暂停，           
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
       #include <sys/time.h>

#define	FULL 1024 * 1024			/*  */

struct {
    pthread_mutex_t lock;
    pthread_cond_t full_cond;
    pthread_cond_t empty_cond;

    int head;
    int tail;
    int size;
    void *data;
    int reset;
} buf = {.lock=PTHREAD_MUTEX_INITIALIZER, 
    .full_cond =PTHREAD_COND_INITIALIZER,
    .empty_cond =PTHREAD_COND_INITIALIZER
};

void log_cancel(void *x)
{
    printf("ener cancel\n");
    int *fd = (int *)x;
    if (*fd == -1)
        return;

    if (buf.head != buf.tail) {
        int n = write(*fd, buf.data+buf.head, buf.tail - buf.head);
        if (n == -1)
            printf("%s\n", strerror(errno));
        else
            printf("logg write %d head:%d tail:%d\n", n, buf.head, buf.tail);

        buf.head = buf.tail = 0;
    }
    close(*fd);

    printf("log cancel\n");
}

void * log_back(void *arg)
{
    int fd=-1;

    struct timeval now;
    struct timespec timeout;

    printf("log_back start\n");

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_cleanup_push(log_cancel, &fd);
    fd = open("./log", O_CREAT | O_RDWR | O_CLOEXEC | O_TRUNC , 0777);
    if (fd == -1) {
        printf("open bad\n");
        return NULL;
    }

    //write 
    while (1) {
        pthread_mutex_lock(&buf.lock);

        gettimeofday(&now, NULL);
        timeout.tv_sec = now.tv_sec+1;
        timeout.tv_nsec = now.tv_usec * 1000;

        while (buf.head == buf.tail) {
            pthread_cond_broadcast(&buf.empty_cond);
            pthread_cond_timedwait(&buf.full_cond, &buf.lock, &timeout);
        }

//        h=buf.head; t=buf.tail;
        int n = write(fd, buf.data+buf.head, buf.tail - buf.head);
        buf.head = buf.tail = 0;
        pthread_mutex_unlock(&buf.lock);

        if (n == -1)
            printf("%s\n", strerror(errno));
        else
            printf("logg write %d head:%d tail:%d\n", n, h, t);

    }

    printf("log_back sleep over\n");
    pthread_cleanup_pop(0);
    return NULL;
}


void write_log(const char* msg, size_t len)
{
    pthread_mutex_lock(&buf.lock);
    while (buf.tail + len > buf.size) {
        printf("buf full work down.....\n");
        pthread_cond_signal(&buf.full_cond);
        pthread_cond_wait(&buf.empty_cond, &buf.lock);
    }

    memcpy(buf.data+buf.tail, msg, len);
    buf.tail += len;

    pthread_mutex_unlock(&buf.lock);
}

void* work(void *arg)
{
    char buf[64];
    printf("work %lu start\n", pthread_self());

    for (int i=0; i< 500000; ++i) {
        int len = snprintf(buf, 64, "thread %lu write a log\n", (unsigned long)pthread_self());
        assert(len < 64);
        buf[len++] = '\0';

        write_log(buf, len);
    }

    return NULL;
}


int
main(int argc, char *argv[])
{
    buf.data = malloc(FULL);
    buf.head = buf.tail = 0;
    buf.size = FULL;

	pthread_t ptid[2];
	pthread_t logtid;

    (pthread_create(&logtid, NULL, log_back, NULL));
    (pthread_create(&ptid[0], NULL, work, NULL));
    (pthread_create(&ptid[1], NULL, work, NULL));

    for (int i=0; i<2; ++i)
        pthread_join(ptid[i], NULL/*not care thread return code */);

    printf("work over\n");

    //close log
    pthread_cancel(logtid);
    printf("cancle log....\n");
    pthread_join(logtid, NULL/*not care thread return code */);

    printf("main join over\n");

    return 0;
}				/* ----------  end of function main  ---------- */
