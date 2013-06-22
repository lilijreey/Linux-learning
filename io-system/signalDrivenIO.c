/**
 * @file     signalDrivenIO.c
 *           信号驱动IO 编程范例
 * @主要步骤
 *    1 建立处理IO signal的函数，通常IO signal为 SIGIO
 *    2 设置 file descriptor 的 owner，告诉kernel当 可读写时
 *        发送signal给哪个process 通过 fcntl F_SETOWN 来设置
 *           @n fcntl(fd, F_SSETOWN, pid);
 *    3. 把fd 设置为非阻塞IO 
 *    4. 开启 singal-driven IO模式. 通过 O_ASYNC status flags
 *         开启后当fd可读写时，就会发送SIGIO 。 可以使用F_SETSIG
 *         来设置发送的信号。
 *         @n flags = fcntl(fd, F_GETFL);
 *         @n fcntl(fd, F_SETFL, flags | O_ASYNC | O_NONBLOCK)
 *
 * singal-driven IO 提供一个edge-triggered 通知。 和epoll ET意思相同
 *       就是说如果fd可读写 内核知会发送一次signal。 应用层需要
 *       保证把fd读完，或写完， 就是直到返回 EAGAIN or EWOULDBLOCK
 *       
 * @note  O_ASYNC 在POSIX.1g 中有但是没有在 SUSv3 中
 *        其他的UNIX 中可能把FASYNC 代替O_ASYNC
 * @author   lili  <lilijreey@gmail.com>
 * @date     06/22/2013 08:14:50 AM
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <zhao/tools.h>

// 使用signal-drive IO 来控制STDIN， 如果键入# 就结束程序

static volatile sig_atomic_t sigioCont = 0;
static void sigio_handle(int sig)
{
//    int ret;
//    char c[100];
    printf("SIGIO recived %d cnt:%d\n", sig, ++sigioCont);

//    ///读完所有的数据
//    while ((ret = read(STDIN_FILENO, &c, 100))) {
//        if (ret == -1) {
//            if (errno == EAGAIN || errno == EWOULDBLOCK) {
//                printf("read over\n");
//                break;
//            } else {
//                perror("read error");
//                exit(1);
//            }
//        }
//        c[ret] = '\0';
//        printf("out:%s\n", c);
//    }
//    sleep(1);
}

int main()
{
    ///1 set signal handle
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = sigio_handle;
    E_TEST(-1, sigaction(SIGIO, &sa, NULL));

    ///2 set owner process
    E_TEST(-1, fcntl(STDIN_FILENO, F_SETOWN, getpid()));

    ///3,4 enable nonblock and singal-drive mode
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    E_TEST(-1, fcntl(STDIN_FILENO, F_SETFL, flags | O_ASYNC | O_NONBLOCK));

    /// loop do other
    for (;;) {
        int j;
        char c[100];
        int ret;
        for (j=0; j < 10000000; j++)
            continue;  //do

        if (sigioCont) {

            ///读完所有的数据
            while ((ret = read(STDIN_FILENO, &c, 100))) {
                if (ret == -1) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        printf("read over\n");
                        break;
                    } else {
                        perror("read error");
                        exit(1);
                    }
                }
                c[ret] = '\0';
                printf("out:%s\n", c);
            }
            sigioCont = 0;
        }
    sleep(1);
//        printf("do other thins\n");
    }

}
