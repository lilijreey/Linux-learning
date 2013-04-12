/*
 * =====================================================================================
 * set vim foldmethod=marker
 *       Filename:  epoll.c
 *
 *    Description:  learn epoll 
 *
 *        Created:  10/31/2012 10:32:39 PM
 * 
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <zhao/tools.h>

// listen + epoll + fork 
// Qus. 1.fork 父子进程是指向同一个epoll 吗？ 
//         Ans.
//         是的，如果在子进程中删除fork前添加的fd，那parent可以受到子进程添加
//         fd的事件， 
//      2.有socket接入时会群惊吗？
//      3.相同的fd可以被加入吗？ see 3
//      4.parent加入的fd，有事件时，chlid会知道吗? 会，只用wait
//


// 3. epoll + fork 后 child parent 添加相同的fd到epoll
//     child parent 向epoll添加数值相同的fd，是可以的，
//     因为epoll允许添加dup(2) 的fd， 不同process打开同一文件，相当与dup
//     但是同一process多次添加同一fd，会报错 see epoll man
//
#if 1 //{{{
int main() 
{
    int epoll_fd = -1; 
    EV_TEST(-1, epoll_fd, epoll_create1(0));

    //epoll 内部会复制一个e

    // EE get read fd
    int pid = fork();
    if (pid == 0) {
        int ofd = open("./epoll.c", O_RDONLY);
        if (ofd == -1) err_exit("open error");
        printf("child start ofd:%d\n", ofd);
        // add new fd EPOLLET 边沿触发
        struct epoll_event e = {EPOLLIN | EPOLLERR |EPOLLET, (epoll_data_t)ofd};
        //epoll 内部会复制一个e
        E_TEST(-1, epoll_ctl(epoll_fd, EPOLL_CTL_ADD, e.data.fd, &e));
        sleep(5);
    }
    else {
        sleep(2);
        int ofd = open("./epoll.c", O_RDONLY);
        if (ofd == -1) err_exit("open error");
        printf("parent start ofd:%d\n", ofd);
        // add new fd EPOLLET 边沿触发
        struct epoll_event e = {EPOLLIN | EPOLLERR |EPOLLET, (epoll_data_t)ofd};
        //epoll 内部会复制一个e
        E_TEST(-1, epoll_ctl(epoll_fd, EPOLL_CTL_ADD, e.data.fd, &e));
    }

    //    close(ofd);// close 掉epoll 中添加的fd epoll会自动删除fd event, 不会触发任何的event

    //close fd epoll will auto clean epoll_fd
    close(epoll_fd);

    return 0;
}
#endif //}}}

// epoll + fork 子进程给epoll添加fd， 父进程wait， 结果可以wait到
//     说明 epoll + fork 后父子引用同一epoll实例
# if 0 ///{{{
int main() 
{
    // EE create a epoll instance 
    // epoll_create 中的参数已经不使用了，
    // epoll_create1 中的参数可以是0和O_CLOEXEC. 0时和epoll_create 相同
    // O_CLOEXEC see man-page open
    int epoll_fd = -1; 
    EV_TEST(-1, epoll_fd, epoll_create1(0));

    // EE set epoll
    // add new fd EPOLLET 边沿触发
    //epoll 内部会复制一个e

    // EE get read fd
    int pid = fork();
    if (pid == 0) {
        printf("child start\n");
        int ofd = open("./epoll.c", O_RDONLY);
        if (ofd == -1) err_exit("open error");
        // add new fd EPOLLET 边沿触发
        struct epoll_event e = {EPOLLIN | EPOLLERR |EPOLLET, (epoll_data_t)ofd};
        //epoll 内部会复制一个e
        E_TEST(-1, epoll_ctl(epoll_fd, EPOLL_CTL_ADD, e.data.fd, &e));
        sleep(5);
    }
    else {
        sleep(2);
        printf("parent, child pid:%d\n", pid);
        struct epoll_event evlist ;
        for (;;) {
            int size=0;
            //通常值等待1个
            EV_TEST(-1, size, epoll_wait(epoll_fd, &evlist, 1, -1));
            printf("size:%d\n", size);
            if (evlist.events & EPOLLRDHUP) {
                printf("EPOLLRDHUP\n");
            }

            if (evlist.events & EPOLLERR) {
                printf("EPOLLERR\n");
            }

            if (evlist.events & EPOLLHUP) {//如果不把产生的HUP的df移除会一直产生
                printf("EPOLLHUP fd:%d\n", evlist.data.fd);
                epoll_ctl(epoll_fd, EPOLL_CTL_DEL, evlist.data.fd, NULL);
                break ;
            }

            if (evlist.events & EPOLLOUT) {
                printf("EPOLLOUT\n");
            }

            if (evlist.events & EPOLLIN) { //pipe 对方close一段不会触发EPOLLIN
                printf("EPOLLIN\n");
                // handld every readly fd
                printf("fd%d\n", evlist.data.fd);
            }
        }
    }

    //    close(ofd);// close 掉epoll 中添加的fd epoll会自动删除fd event, 不会触发任何的event

    //close fd epoll will auto clean epoll_fd
    close(epoll_fd);

    return 0;
}
#endif //}}}



