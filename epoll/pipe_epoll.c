/*
 * =====================================================================================
 *
 *       Filename:  pipe_epoll.c
 *
 *    Description:  //通过pipe 链接父子process
 *                  一方crash另一方epoll_wait 的返回event type
 *
 *        Created:  12/22/2012 02:07:19 PM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/epoll.h>

#define	E_TEST(err, call) \
    do{                        \
        if((err == (call))) { \
            perror(#call);     \
            exit(1);           \
        }                      \
    }while(0)


//parent send
static int send_pipe[2];
static int recv_pipe[2];

inline static 
void init_pipe(int pip[2])
{
    E_TEST(-1, pipe(pip));
}

void child_run()
{
    E_TEST(-1, close(send_pipe[1]));
    E_TEST(-1, close(recv_pipe[0]));
    //  int sendfd = recv_pipe[1];
    int recvfd = send_pipe[0];

    char buf[256];
    int len;
    //    while ((len = read(recvfd, buf, 256))) {
    //        buf[len]='\0';
    //        printf("child read [len:%d] [%s]\n", len, buf);
    //    }
    //    if (len == -1) {
    //        perror("child read error");
    //        exit(1);
    //    }
    //    if (len == 0) {
    //        sleep(1);
    //        printf("praent close peer\n");
    //    }


    int epoll_fd = epoll_create1(0);
    struct epoll_event e = {EPOLLOUT | EPOLLIN | EPOLLRDHUP,  (epoll_data_t)recvfd};
    E_TEST(-1, epoll_ctl(epoll_fd, EPOLL_CTL_ADD, e.data.fd, &e));

    for (; ;) 
    {
        int rt = epoll_wait(epoll_fd, &e, 1, -1);
        printf("sssssssssss\n");
        if (rt == -1) {
            perror("epoll_wait error:");
            exit(1);
        }
        if (e.events & EPOLLRDHUP) {
            printf("EPOLLRDHUP\n");
        }

        if (e.events & EPOLLERR) {
            printf("EPOLLERR\n");
        }

        if (e.events & EPOLLHUP) {//如果不把产生的HUP的df移除会一直产生
            printf("EPOLLHUP\n");
            epoll_ctl(epoll_fd, EPOLL_CTL_DEL, e.data.fd, NULL);
        }

        if (e.events & EPOLLOUT) {
            printf("EPOLLOUT\n");
        }

        if (e.events & EPOLLIN) { //pipe 对方close一段不会触发EPOLLIN
            printf("EPOLLIN\n");
            len = read(e.data.fd, buf,256);
            if (len == -1) {
                perror("reand -1");
                exit(1);
            }
            if (len == 0) {
                sleep(1);
                printf("praent close peer\n");
                E_TEST(-1, close(recvfd)); //epoll will auto remove closed fd
                break;
            }
            buf[len] = '\0';
            printf("child read[len;%u] %s\n", len, buf);
        }

    }
    exit(0);
}

void parent_run()
{
    char buf[256];
    E_TEST(-1, close(send_pipe[0]));
    E_TEST(-1, close(recv_pipe[1]));
    int sendfd = send_pipe[1];
    //  int recvfd = recv_pipe[0];


    int len=0;
    //send data to client
    for (int i=0; i<6; ++i) {
        sprintf(buf, "i:%d slen%d", i, len);
        len = write(sendfd, buf, strlen(buf));
        if (len == -1) {
            perror("write");
            exit(1);
        }
        sleep(1);
    }
    close(sendfd);
    //abort();

    E_TEST(-1, wait(NULL));
}

int main(int argc, char *argv[])
{
    //通过pipe 链接
    init_pipe(send_pipe);
    init_pipe(recv_pipe);

    pid_t chlid_pid;
    chlid_pid = fork();
    if (chlid_pid == -1) {
        perror("fork");
        exit(1);
    } else if (chlid_pid > 0) { //parent
        parent_run();
    } else {
        child_run();
    }

    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */


