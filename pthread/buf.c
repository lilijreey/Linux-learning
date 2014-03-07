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

#define	FULL 1024 * 1024/*  */
#define	BF 128			/*  */

typedef struct car_s{
 pthread_mutex_t *lock;
 pthread_cond_t *empty;

 int fd;
 int tail;
 struct car_s *next; 
 void *buf;

} car;


typedef struct event_s
{
    char type; //'f' full, 'c' close
    car *c;
} event_t;


struct {
    pthread_mutex_t lock;
    pthread_mutex_t list_lock;
    pthread_cond_t event;

    car *list; 
    int head;
    int tail;
    int size;
    void *data;
} buf = {.lock=PTHREAD_MUTEX_INITIALIZER, 
    .list_lock=PTHREAD_MUTEX_INITIALIZER,
    .event=PTHREAD_COND_INITIALIZER,
    .list=NULL,
};

car* new_buf(const char *file)
{
    pthread_mutex_t *lock;
    pthread_cond_t *full;
    pthread_cond_t *empty;

    int fd = open(file, O_CREAT | O_RDWR | O_CLOEXEC | O_TRUNC , 0777);
    if (fd == -1) {
        printf("open bad\n");
        return NULL;
    }

    car *c = malloc(sizeof (car));

    lock = malloc(sizeof (pthread_mutex_t));
    full = malloc(sizeof (pthread_cond_t));
    empty = malloc(sizeof (pthread_cond_t));

    pthread_mutex_init(lock, NULL);
    pthread_cond_init(full, NULL);
    pthread_cond_init(empty, NULL);

    c->buf= malloc(FULL);
    c->tail = 0;

    c->lock = lock;
    c->empty= empty;

    c->fd = fd;

    pthread_mutex_lock(&buf.list_lock);
    c->next=buf.list;
    buf.list = c;
    pthread_mutex_unlock(&buf.list_lock);

    return c;
}


void rm_from_list(car *c)
{
    if (buf.list == c) {
        buf.list = c->next;
        return;
    }

    car *n=buf.list;
    while (n->next != c && n->next != NULL)
        n = n->next;

    if (n->next == NULL) {
        assert(0);
        return;
    }

    n->next = c->next;
}

void flush_log(car *c)
{
    int n;
    assert(c->fd);

    pthread_mutex_lock(c->lock);
    if (c->tail != 0) {
        n = write(c->fd, c->buf, c->tail);
        if (n == -1)
            perror("xxx write failed");
        c->tail = 0;
    }
    pthread_mutex_unlock(c->lock);

}

void car_destroy(car *c)
{
                rm_from_list(c); 
                flush_log(c);
                pthread_cond_destroy(c->empty); 
                pthread_mutex_destroy(c->lock);

                if (c->fd > 2)
                    close(c->fd);

                free(c->buf);
                free(c);

}

void log_cancel(void *x)
{
    printf("log cancel\n");
    car *c= buf.list;
    while (c) {
        printf("des :%d\n", c->fd);
        car_destroy(c);
        c=c->next;
    }

}




void flush_logs()
{
    car *c= buf.list;
    while (c) {
        flush_log(c);
        c=c->next;
    }
}



void * log_back(void *arg)
{
    int fd=-1;

    struct timeval now;
    struct timespec timeout;
    event_t *e;
    car *c;
    int n=0, ret=0;
    char arr[BF];
    int bl, hl;

    printf("log_back start\n");

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_cleanup_push(log_cancel, &fd);

    //write 
    while (1) {
        gettimeofday(&now, NULL);
        timeout.tv_sec = now.tv_sec+1;
        timeout.tv_nsec = now.tv_usec * 1000;

        pthread_mutex_lock(&buf.lock);
        while (buf.head == buf.tail && ret != ETIMEDOUT)   {
            printf("event wait\n");
            ret = pthread_cond_timedwait(&buf.event, &buf.lock, &timeout);
        }
        ret=0;

        //dup event
        memcpy(arr, buf.data, buf.tail);
        bl = buf.tail;
        hl=0;
        buf.head = buf.tail = 0;

        pthread_mutex_unlock(&buf.lock);



        while (hl != bl) { //pop event 
            e= (event_t *)(arr+hl);
            hl += sizeof (event_t);

            switch (e->type) {
            case 'c':
                car_destroy(e->c); 
                break;
            case 'f':
                c=e->c;
                pthread_mutex_lock(c->lock);
                n = write(c->fd, c->buf, c->tail);
                if (n == -1)
                    perror("write failed");
                c->tail=0;
                pthread_mutex_unlock(c->lock);
//                printf("broadcast\n");
                pthread_cond_broadcast(c->empty);
                break;
            }
        }

//        printf("llllllllllll fuls\n");
        flush_logs();

    }

    printf("log_back sleep over\n");
    pthread_cleanup_pop(0);
    return NULL;
}


void push_full_event(car *c)
{
//    printf("push full tail:%d\n", buf.tail);
    event_t *e;
    // TODO buf even full condition ??
    pthread_mutex_lock(&buf.lock);
    e = (event_t*)(buf.data + buf.tail);
    e->c=c;
    e->type='f';
    buf.tail += sizeof(event_t);
    pthread_mutex_unlock(&buf.lock);
}

void push_close_event(car *c)
{
    printf("close full\n");
    event_t *e;
    //buf even full condition ??
    pthread_mutex_lock(&buf.lock);
    e = (event_t*)(buf.data + buf.tail);
    e->c=c;
    e->type='c';
    buf.tail += sizeof(event_t);
    pthread_mutex_unlock(&buf.lock);
}

void write_log(car *c, const char* msg, size_t len)
{
    pthread_mutex_lock(c->lock);
    while (c->tail + len > FULL) {
        printf("buf full work down.....\n");
        push_full_event(c);
        pthread_cond_signal(&buf.event);
        pthread_cond_wait(c->empty, c->lock);
        printf("work buf empty\n");
    }

    memcpy(c->buf +c->tail, msg, len);
    c->tail += len;

    pthread_mutex_unlock(c->lock);
}

void* work(void *arg)
{
    car *c = (car*)arg;
    char buf[64];
    printf("work %lu start\n", pthread_self());

    for (int i=0; i< 500000; ++i) {
        int len = snprintf(buf, 64, "thread %lu write a log\n", (unsigned long)pthread_self());
        assert(len < 64);
        buf[len++] = '\0';

        write_log(c, buf, len);
    }

    return NULL;
}


int
main(int argc, char *argv[])
{
    buf.data = malloc(BF);
    buf.head = buf.tail = 0;
    buf.size = BF;
    buf.list = NULL;

	pthread_t ptid[2];
	pthread_t logtid;

    car *xx = new_buf("./log");
    car *yy = new_buf("./nn");
    assert(xx);

    (pthread_create(&logtid, NULL, log_back, NULL));
    (pthread_create(&ptid[0], NULL, work, xx));
    (pthread_create(&ptid[1], NULL, work, yy));

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
