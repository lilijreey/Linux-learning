/*
 * =====================================================================================
 *
 *       Filename:  epoll.c
 *
 *    Description:  learn epoll 
 *
 *        Created:  10/31/2012 10:32:39 PM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */

/*
   reference man epoll,
 * 
 */

#include <unistd.h>
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <zhao/tools.h>

#define EPOLL_LIST_SIZE  5

int main() 
{
    // EE create a epoll instance 
    // epoll_create 中的参数已经不使用了，
    // epoll_create1 中的参数可以是0和O_CLOEXEC. 0时和epoll_create 相同
    // O_CLOEXEC see man-page open
    int epoll_fd = -1; 
    EV_TEST(-1, epoll_fd, epoll_create1(0));

    int ofd = open("./epoll.c", O_RDONLY);
    if (ofd == -1) err_exit("open error");
    // EE set epoll
    // add new fd
    struct epoll_event e = {EPOLLIN | EPOLLET, (epoll_data_t)ofd};

    E_TEST(-1, epoll_ctl(epoll_fd, EPOLL_CTL_ADD, e.data.fd, &e));

    // EE get read fd
    struct epoll_event evlist[EPOLL_LIST_SIZE] ;
    int size=0;
    for (;;) {
        //通常值等待1个
        //epoll_wait(epoll_fd, evlist, 1, -1));
        EV_TEST(-1, size, epoll_wait(epoll_fd, evlist, EPOLL_LIST_SIZE, -1));
        printf("size:%d\n", size);
        // handld every readly fd
        int i;
        for (i=0; i < size; ++i) {
            printf("fd%d\n", evlist[i].data.fd);
        }
    }

    close(epoll_fd);

    return 0;
}

