/*
 * =====================================================================================
 *
 *       Filename:  alarm.c
 *
 *    Description:  计时器的使用和技术
 *
 *        Version:  1.0
 *        Created:  04/20/2011 08:21:42 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#include	<unistd.h>
#include	<time.h>
#include	<sys/types.h>
#include	<sys/time.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<signal.h>
#include	<zhao/tools.h>

const char* const signame[] = {
	"SIG_0",
	"SIGHUP",	"SIGINT",	"SIGQUIT",	"SIGILL",
	"SIGTRAP",	"SIGABRT",	"SIGBUS",	"SIGFPE",
	"SIGKILL",	"SIGUSR1",	"SIGSEGV",	"SIGUSR2",
	"SIGPIPE",	"SIGALRM",	"SIGTERM",	"SIGSTKFLT",
	"SIGCHLD",	"SIGCONT",	"SIGSTOP",	"SIGTSTP",
	"SIGTTIN",	"SIGTTOU",	"SIGURG",	"SIGXCPU",
	"SIGXFSZ",	"SIGVTALRM","SIGPROF",	"SIGWINCH",
	"SIGIO",	"SIGPWR",	"SIGSYS",
};

static void sigalrm_handle(int signo) 
{
	printf("send signal:%s\n", signame[signo]) ;
}

static void sigusr1_handle(int signo) 
{
	printf("send signal:%s\n", signame[signo]) ;
}

// call back function type
typedef void (callback_t)(int) ;
void cb_func(int i) {
    if (i % 1000 == 0)
        printf("call back i:%d time:%lu\n", i, time(NULL));
}

// 心跳功能，周期执行一些事情
// 分别使用alarm, setitimer, timer_create 实现
// 几个问题:
//      1.由于使用的是pause所以收到任何一个signal都会唤醒线程
// 可能导致调用不正确，使用alarm的因为可以检测alarm的返回时间，所以没有
// 这个问题。 setitimer, timer_create可以在使用高级的signal函数来检测,
//      2.多个相同signal导致调用错误.可以通过时间检测来避免

// use alarm, HZ is 1
// 总结：alarm有缺陷，达不到工业级标准。
// 不足：由于精度太小，如果在1秒内受到多个SIGARML，
//       就会导致 alarm返回同一个值，其实已经有时间流逝了。但是，
//       alarm还会设置没有减小的值，导致实际两个心跳直接的间隔大于5秒，
//       甚至不会执行回调函数
void alarm_heartbeat(callback_t callback)
{
    //这里还需阻塞其他信号
	E_TEST(SIG_ERR , signal(SIGALRM, sigalrm_handle)) ;
    for (int t=0;; ++t) {
        unsigned int sec = 5;
        // 保证在5秒内多次受到sigalrm 只会设置1次调用
        while ((sec = alarm(sec))) {
            // if sec != 0 run this 
            printf("up remain:%u\n", sec);
            pause(); //suppest  until resave a signal
        }
        // sec == 0
        callback(t);
        pause(); // 这个pause保证正确性
    }
}

// use setitimer
// 使用: it_value 设置第一次定时的时间，
//       interval设置重新启动后定时的时间。如果为0就不会重新启动
//       常用与分析程序的执行时间效率
// 总结：优点提供自动重启，精度高。
//       不足，使用信号，不能区分是否是setitmer发出的信号，也不会
//       返回剩余秒数
//       平率越高cpu越高，(HZ=1000 cpu %4) 开启优化几乎没有作用

void setitimer_heartbeat(callback_t callback)
{
    // HZ is 1000 对于1000 来说机器可以及时的发送signal，虽然机器的
    // HZ 是100，可能用的是高精度时钟
    const unsigned int TT = 1000;
    struct itimerval delay;
    delay.it_value.tv_sec = 0;
    delay.it_value.tv_usec= TT; // 10^-6

    //设置重新启动的定时时间,如果为0就不会重新启动 
    delay.it_interval.tv_sec = 0;
    delay.it_interval.tv_usec= TT;

	E_TEST(SIG_ERR , signal(SIGALRM, sigalrm_handle));
    E_TEST(-1, setitimer(ITIMER_REAL, &delay, NULL));
    for ( int t =1; ; ++t) {
        //ITIMER_REAL he alarm 一样都是设定墙上时间
        pause(); //suppest  until resave a signal
        callback(t);
    }
}

// use timer_create, timer_settime
// 可以指定发送的信号
// 默认为poxis间歇定时器的方式通知
// see man page 系统为每一个timer准备一个实时信号队列
//
void posix_timer_heartbeat(callback_t callback)
{
    // first step: create a timer, not set
    timer_t timer;
    struct sigevent evp;
    memset(&evp, 0, sizeof(evp));
    evp.sigev_value.sival_ptr = &timer;
    evp.sigev_notify = SIGEV_SIGNAL; // 设置到时行为 发送信号
    evp.sigev_signo = SIGUSR1; // 设置发送信号ID SIGUSR1 信号

	E_TEST(SIG_ERR , signal(SIGUSR1, sigusr1_handle));
    E_TEST(-1, timer_create(CLOCK_REALTIME,
                            &evp, // 定义当定时器到期时的异步通知
                    //如果为NULL，将发送SIGALRM相当于
                    //  evp.sigev_value.sival_ptr = &timer;
                    //  evp.sigev_notify = SIGEV_SIGNAL;
                    //  evp.sigev_signo = SIGARML; 
                            &timer));

    // second set timer. 关联定时时间，并开始记时
    const unsigned int TT = 10000000; //HZ 1000
    struct itimerspec val;

}

int main()
{
//    alarm_heartbeat(cb_func);
    //setitimer_heartbeat(cb_func);
    posix_timer_heartbeat(cb_func);

    return 0;
}
