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
 * @note  1. O_ASYNC 在POSIX.1g 中有但是没有在 SUSv3 中
 *           其他的UNIX 中可能把FASYNC 代替O_ASYNC
 *
 *        2. 必须在开启singla-drive 模式前必须设置 SIGIO 的handle
 *           因为SIGIO 默认的操作是terminal process
 *           
 *        3. 对于不同类型的 fd 发送SIGIO 的情形不同
 *           终端和伪终端：
 *               不论是否有输入都发送 SIGIO
 *           pipe, FIFO
 *              read 
 *              write
 *               todo
 *          socket
 *             UDP 
 *               TODO
 *             TCP 
 *               TODO
 *
 *   4 发挥signal-derive IO 的最大特性
 *     1 使用F_SETSIG 设置一个实时signal来代替 SIGIO
 *       因为SIGIO 是没有队列的，多个SIGIO可能会被求其掉从而溜掉了可以操作的fd
 *            fcntl(fd, F_SETSIG, sig)
 *            
 *     2 指定 SA_SIGINFO flag when using sigaction
 *      
 *
 *   
 *        
 * @Qus. 
 *      1. 如何区分fd是可读还是可写？
 *      2. 如何知道是哪个fd？
 *        使用 siginfo_t 结构，从中得到相关内容
 *
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
    int ret;
    char c[100];
    printf("SIGIO recived %d cnt:%d\n", sig, ++sigioCont);

    ///读完所有的数据
    ((ret = read(STDIN_FILENO, &c, 100))) ;
    {
        if (ret == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                printf("read over\n");
            } else {
                perror("read error");
                exit(1);
            }
        }
        c[ret] = '\0';
        printf("out:%s\n", c);
    }
}

int main()
{
    // 终端会不断的发送SIGIO 不管有没有输入可读
    ///1 set signal handle
    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = sigio_handle;
    E_TEST(-1, sigaction(SIGIO, &sa, NULL));

    ///2 set owner process
    E_TEST(-1, fcntl(STDIN_FILENO, F_SETOWN, getpid()));
    // fcntl(fd, F_GETOWN) -> int 返回fd的pid 如果是正数就是pid，
    //  如果是负数就是 process group id

    ///3,4 enable nonblock and singal-drive mode
    int flags = fcntl(STDIN_FILENO, F_GETFL);
    E_TEST(-1, fcntl(STDIN_FILENO, F_SETFL, flags | O_ASYNC | O_NONBLOCK));

    /// loop do other
    for (;;) {
        int j;
        for (j=0; j < 10000000; j++)
            continue;  //do

    sleep(1);
    }

}
