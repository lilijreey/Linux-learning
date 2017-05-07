/*
 * =====================================================================================
 *
 *       Filename:  posix_clock.c
 *
 *    Description:  POSIX 标准函数
 *
 *        Created:  08/14/2012 09:48:04 PM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */

// 得到POSIX时间源的精度
// 使用clock_getres 

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#include "zhao/tools.h"

clockid_t clocks[] = {
    CLOCK_REALTIME,
    CLOCK_MONOTONIC,
    CLOCK_PROCESS_CPUTIME_ID,
    CLOCK_THREAD_CPUTIME_ID,
    (clockid_t)-1
};
//时间精度都是1纳秒，但是机器HZ是10000纳秒，说明使用了一种更高精度的时间源
//在X86上就是TSC
void getprecision()
{
    //POSIX 定义的四种时间源


    struct timespec res;
    int i;
    for (i = 0; clocks[i] != (clockid_t)-1; ++i) {
        if (-1 == clock_getres(clocks[i], &res))
            perror("clock_getres error");
        else
            printf("clock:%d, sec:%ld, nsec:%ld\n", 
                   i, (long)res.tv_sec, (long)res.tv_nsec);
    }
}


//get wall time
void get_wall_time()
{
    //1
    printf("time: sec:%lu\n", time(NULL));
    
    //2
    struct timeval res;
    //第二个参数已经废弃，使用NULL就好
    if (-1 ==gettimeofday(&res, NULL))
        perror("gettimeofday error");
    printf("gettimeofday: sec:%lu, usec:%lu\n", res.tv_sec, res.tv_usec);

    //3 
    printf("clock_gettime:\n");
    struct timespec val;
    for (int i = 0; clocks[i] != (clockid_t)-1; ++i) {
        if (-1 == clock_gettime(clocks[i], &val))
            perror("clock_gettime error");
        else
            printf("clock:%d, sec:%ld, nsec:%ld\n", i, val.tv_sec, val.tv_nsec);
    }
}


void set_time()
{
    //1 stime
    time_t t=100;
    E_TEST(-1, stime(&t));
}

//睡眠x秒，中间如果有信号中断睡眠则会继续睡眠
//前提：该信号不会导致进程中断
void sleep_until()
{
    int sec =16;
    printf("sleep begin\n");
    while ((sec = sleep(sec))) printf("sleep interrupt\n");
    printf("sleep end\n");
}

int main()
{
    //getprecision();
    //set_time();
    //get_wall_time();
    sleep_until();
    
    

    return 0;
}
