/**
 * @file     gettid.c
 *           实现gettid() 返回一个线程的tid
 *           
 * @author   lili  lilijreey@gmail.com
 * @date     05/06/2013 11:09:30 AM
 *
 * * gettid/0 之在Linux下有效
 * * 对于只有一个线程的进程他的tid=pid，但是对于有多个线程的进程，
 *     每一个线程的pid都相同，但是有各自的tid。
 * * note gettid/0 返回的值和 pthread_self/0 返回的不同的标记.
 *  
 * * glibc 并没有实现gettid, 只有系统syscall,必须自己实现gettid/0
 *   
 * * gettid/0  pthread_self/0 的区别，
 *         gettid返回的是clone 自己的id，
 *         在tid在同一主机上是唯一的, pthread_self/0只在同一进程中唯一，而且
 *         新的pthread 可能和以死亡的pthread 返回相同的值
 */

#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>

pid_t gettid()
{
  return syscall(SYS_gettid);
}

int
main(int argc, char *argv[])
{
    printf("tid:%u pid:%u\n", gettid(), getpid());
    return 0;
}

