/**
 * @file     sigwait.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     02/08/2014 03:23:19 PM
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>


#if 0 //给一个进程发送信号，线程会如何
// 所有线程设置的handle都共享，
// 给一个进程发送信号时，只有一个线程处理signal, 随即的线程,不固定

pthread_t pp[5];
pid_t          pid; 

void hand_sig(int sig)
{
    printf("thread:%ld recive sig:%d\n", (long)pthread_self(), sig);
}

void* wait_thread()
{
    printf("create %ld thread\n", pthread_self());
    kill(pid, SIGALRM);

    if (pthread_self() == pp[2]) {
        if (SIG_ERR == signal(SIGINT, hand_sig)) {
            perror("signal error");
        }

    } else if (pthread_self() == pp[3]) {
        if (SIG_ERR == signal(SIGHUP, hand_sig)) {
            perror("signal error");
        }
    }

    for (;;)
        pause();

    return 0;

}



int main()
{
    pid = getpid();
    printf("master pid:%d\n", pid);

    if (SIG_ERR == signal(SIGTERM, hand_sig)) {
        perror("signal error");
    }
    if (SIG_ERR == signal(SIGALRM, hand_sig)) {
        perror("signal error");
    }

    for (int i =0; i<5; ++i)
        pthread_create(&pp[i], NULL, wait_thread, NULL);

    for (;;)
        pause();
    return 0;
}


#endif

// 多线程程序同步处理信号模型
// 1. 主线程设置希望处理的sigmaks， 并屏蔽他们
// 2. 这种这些sigmask的handle
// 3. 创建一个线程专门来处理signal
// 4. 创建其他工作线程
#if 0

void sig_handler(int signum)
{
    printf("Receive signal. %d\n", signum);
}

void* sigmgr_thread()
{
    sigset_t   waitset; 
    int        sig;
    int        rc;
    pthread_t  ppid = pthread_self();

    pthread_detach(ppid);

    sigemptyset(&waitset);
    sigaddset(&waitset, SIGRTMIN);
    sigaddset(&waitset, SIGRTMIN+2);
    sigaddset(&waitset, SIGRTMAX);
    sigaddset(&waitset, SIGUSR1);
    sigaddset(&waitset, SIGUSR2);

    while (1)  {
        rc = sigwait(&waitset, &sig);
        if (rc != -1) {
            sig_handler(sig);
        } else {
            printf("sigwaitinfo() returned err: %d; %s\n", errno, strerror(errno));
        }
    }
}


int main()
{
    sigset_t bset, oset;
    pid_t           pid = getpid();
    pthread_t       ppid;

    sigemptyset(&bset);
    sigaddset(&bset, SIGRTMIN);
    sigaddset(&bset, SIGRTMIN+2);
    sigaddset(&bset, SIGRTMAX);
    sigaddset(&bset, SIGUSR1);
    sigaddset(&bset, SIGUSR2);

    if (pthread_sigmask(SIG_BLOCK, &bset, &oset) != 0)
        printf("!! Set pthread mask failed\n");

    kill(pid, SIGRTMAX);
    kill(pid, SIGRTMAX);
    kill(pid, SIGRTMIN+2);
    kill(pid, SIGRTMIN);
    kill(pid, SIGRTMIN+2);
    kill(pid, SIGRTMIN);
    kill(pid, SIGUSR2);
    kill(pid, SIGUSR2);
    kill(pid, SIGUSR1);
    kill(pid, SIGUSR1);

    // Create the dedicated thread sigmgr_thread() which will handle signals synchronously
    pthread_create(&ppid, NULL, sigmgr_thread, NULL);

    sleep(10);

    return 0;
}

#endif


#if 1 //333
void sig_handler(int signum)
{
    static int j = 0;
    static int k = 0;
    pthread_t  sig_ppid = pthread_self(); 
    // used to show which thread the signal is handled in.
   
    if (signum == SIGUSR1) {
        printf("thread %ld, receive SIGUSR1 No. %d\n", (long)sig_ppid, j);
        j++;
    //SIGRTMIN should not be considered constants from userland, 
    //there is compile error when use switch case
    } else if (signum == SIGRTMIN) {
        printf("thread %ld, receive SIGRTMIN No. %d\n", (long)sig_ppid, k);
        k++;
    }
}

void* worker_thread()
{
    pthread_t  ppid = pthread_self();
    pthread_detach(ppid);
    while (1) {
        printf("I'm thread %ld, I'm alive\n", (long)ppid);
        sleep(10);
    }
}

void* sigmgr_thread()
{
    sigset_t   waitset; 
    siginfo_t  info;
    int        rc;
    pthread_t  ppid = pthread_self();

    pthread_detach(ppid);

    sigemptyset(&waitset);
    sigaddset(&waitset, SIGRTMIN);
    sigaddset(&waitset, SIGUSR1);

    while (1)  {
        rc = sigwaitinfo(&waitset, &info);
        if (rc != -1) {
            printf("sigwaitinfo() fetch the signal - %d\n", rc);
            sig_handler(info.si_signo);
        } else {
            printf("sigwaitinfo() returned err: %d; %s\n", errno, strerror(errno));
        }
    }
}


int main()
{
    sigset_t bset, oset;
    int             i;
    pid_t           pid = getpid();
    pthread_t       ppid;
    siginfo_t xx;
    

    // Block SIGRTMIN and SIGUSR1 which will be handled in 
    //dedicated thread sigmgr_thread()
    // Newly created threads will inherit the pthread mask from its creator 
    sigemptyset(&bset);
    sigaddset(&bset, SIGRTMIN);
    sigaddset(&bset, SIGUSR1);
    if (pthread_sigmask(SIG_BLOCK, &bset, &oset) != 0)
        printf("!! Set pthread mask failed\n");
    
    // Create the dedicated thread sigmgr_thread() which will handle 
    // SIGUSR1 and SIGRTMIN synchronously
    pthread_create(&ppid, NULL, sigmgr_thread, NULL);
  
    // Create 5 worker threads, which will inherit the thread mask of
    // the creator main thread
    for (i = 0; i < 5; i++) {
        pthread_create(&ppid, NULL, worker_thread, NULL);
    }

    // send out 50 SIGUSR1 and SIGRTMIN signals
    for (i = 0; i < 50; i++) {
        kill(pid, SIGUSR1);
        printf("main thread, send SIGUSR1 No. %d\n", i);
        kill(pid, SIGRTMIN);
        printf("main thread, send SIGRTMIN No. %d\n", i);
        sleep(10);
    }
    return 0;
}

#endif
