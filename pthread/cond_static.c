/*
 * =====================================================================================
 *
 *       Filename:  cond_static.c
 *
 *    Description:  contand variable
 *
 *        Created:  03/03/2012 11:45:37 AM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */
#include	<stdio.h>
#include	<unistd.h>
#include	<pthread.h>
#include	<stdlib.h>
#include	<zhao/tools.h>


//pthread_cond_t static initaliztion and use pthread_cond_init


struct node {
	pthread_mutex_t mu;
	pthread_cond_t co ;
	int i ;
} data= { PTHREAD_MUTEX_INITIALIZER,  PTHREAD_COND_INITIALIZER, 0};

/// 连个线程，一个++，一个--，加到10，通知--，减到0，睡眠等等通知

#define	TIME 1			/*  */
#define	FULL 10			/*  */
#define	NONE 0			/*  */

/// condiction variable 的作用的发出信号
void *
wait_thread(void *arg) {
	struct timespec timeout ;
	timeout.tv_sec = time(NULL) + 1000;
	timeout.tv_nsec = 0;
	for (;;) {

	PCHECK(pthread_mutex_lock(&data.mu));
	++data.i;
	printf("c data++ :%d\n", data.i);
		PCHECK(pthread_cond_signal(&data.co));
	/// 必须持有mutxA是，才能waitmutex，否则死锁
	while (data.i >= FULL) {// production is full signal x and sleep
		printf("c s\n");
		// 等代条件变量，总是返回锁住的mutex，
		PCHECK(pthread_cond_signal(&data.co));
		printf("c w\n");
		PCHECK(pthread_cond_timedwait(&data.co, &data.mu, &timeout));
		// 别唤醒是继续持有mutex
	}

	PCHECK(pthread_mutex_unlock(&data.mu));
	sleep(TIME);

	///发送是其他等代的thread并不会马上执行，因为还有mutex没释放
	}

	return NULL;
}

void *
wait_thread1(void *arg) {
	struct timespec timeout ;
	timeout.tv_sec = time(NULL) + 1000;
	timeout.tv_nsec = 0;
	for (;;) {

	PCHECK(pthread_mutex_lock(&data.mu));
	++data.i;
	printf("c1 data++ :%d\n", data.i);
		PCHECK(pthread_cond_signal(&data.co));
	/// 必须持有mutxA是，才能waitmutex，否则死锁
	while (data.i >= FULL) {// production is full signal x and sleep
		printf("c1 s\n");
		// 等代条件变量，总是返回锁住的mutex，
		PCHECK(pthread_cond_signal(&data.co));
		printf("c1 w\n");
		PCHECK(pthread_cond_timedwait(&data.co, &data.mu, &timeout));
		// 别唤醒是继续持有mutex
	}

	PCHECK(pthread_mutex_unlock(&data.mu));
	sleep(TIME);

	///发送是其他等代的thread并不会马上执行，因为还有mutex没释放
	}

	return 0;
}

int main(int argc, char *argv[]) {

//	pthread_cond_t co ;
//	pthread_cond_init(&co, NULL/*attr*/);
//	pthread_cond_destroy(&co);

	struct timespec timeout ;
	timeout.tv_sec = time(NULL) + 1000;
	timeout.tv_nsec = 0;
	pthread_t pt, pt1;

	PCHECK(pthread_create(&pt, NULL, wait_thread, NULL));
	PCHECK(pthread_create(&pt1, NULL, wait_thread1, NULL));

	for (;;) {
		PCHECK(pthread_mutex_lock(&data.mu));
		// 调用以后，会释放原来占有的mutx

		if (data.i >0) {
			--data.i;
			printf("f dec data: %d\n", data.i);
		} else {
			while (data.i == 0) {
				//!!! 在wait和唤醒是都检测predict 所以总是把它放在while中
			printf("f s\n");
			PCHECK(pthread_cond_signal(&data.co));
			printf("f w\n");
			PCHECK(pthread_cond_timedwait(&data.co, &data.mu, &timeout));
			}
		}
		PCHECK(pthread_mutex_unlock(&data.mu));

		//sleep(TIME);
	}
	return 0 ;
}
