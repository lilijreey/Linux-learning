/* vim: foldmethod=marker
 * =====================================================================================
 *
 *       Filename:  create_thread.c
 *
 *    Description:  
 *
 *        Created:  12/07/2011 11:15:53 PM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */

#include  <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h> //for pthread_kill
#include	<sys/types.h>
#include	<pthread.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<zhao/tools.h>

#define COUNT 100000

/// 比较2个线程同时写一个共享数据，不是用锁 造成race conditions(条件竞争)
//  和使用锁时的情况

//输出情况
//thread:140541228873472 no_lock_sum:100000
//thread:140541220480768 no_lock_sum:100000
//thread:140541203695360 lock_sum:190932
//thread:140541212088064 lock_sum:200000
//
//no_lock_sum:100000 //不加锁的明显不对，有可能是每个线程把sum暂存到了寄存器中++，最后在写入sum
//lock_sum:200000  //加锁的每次lock mutex 时都会同步不同线程的内存视图
//
//使用 `volatile int no_lock_sum' is also of no avail.

int no_lock_sum;
int lock_sum;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER ;
void* no_lock_add(void *arg)
{
    for (int i=0; i< COUNT; ++i)
        ++no_lock_sum;
    printf("thread:%lu no_lock_sum:%d\n", (unsigned long)pthread_self(), no_lock_sum);
    return 0;
}

/// 2个线程同时写一个共享数据，使用锁保护
void* lock_add(void *arg)
{
    for (int i=0; i< COUNT; ++i) {
		PCHECK(pthread_mutex_lock(&mutex)) ;
        ++lock_sum;
		PCHECK(pthread_mutex_unlock(&mutex)) ;
    }
    printf("thread:%lu lock_sum:%d\n", (unsigned long)pthread_self(), lock_sum);
    return 0;
}

void* detach_p(void *a  )
{
    printf("thread sleep\n");
    sleep(5);
    return 0;
}

int main(void)
{
	pthread_t ptid[4];
    PCHECK(pthread_create(&ptid[0], NULL, no_lock_add, NULL));
    PCHECK(pthread_create(&ptid[1], NULL, no_lock_add, NULL));
    PCHECK(pthread_create(&ptid[2], NULL, lock_add, NULL));
    PCHECK(pthread_create(&ptid[3], NULL, lock_add, NULL));


    for (int i=0; i<4; ++i)
        pthread_join(ptid[i], NULL/*not care thread return code */);

    printf("no_lock_sum:%d\n", no_lock_sum);
    printf("lock_sum:%d\n", lock_sum);


//    PCHECK(pthread_create(&ptid[0], NULL, detach_p, NULL));
//    PCHECK(pthread_detach(ptid[0]));
//    PCHECK(pthread_kill(ptid[0], SIGKILL));

    sleep(7);
    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
