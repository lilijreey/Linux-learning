/*
 * =====================================================================================
 *
 *       Filename:  mutex.c
 *
 *    Description:  
 *
 *        Created:  03/02/2012 08:19:44 PM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */
#include	<stdio.h>
#include	<unistd.h>
#include	<pthread.h>
//#include	<stdlib.h>
#include	<string.h>
#include	<zhao/tools.h>

/* use mutex */
// mutex 规定
// 不能解锁没有枷锁的mutex，
// 也不能解锁其他线程枷锁的mutex
// (一个mutex 最多只能加锁一次)已经加锁mutex的线程不能在次加锁同一个mutex
//
// 多个线程争抢一个mutex，只有一个加速成功，其他都线程
// 都会阻塞到lock调用，直到，mutex unlock，
//
// 可以使用 pthread_mutex_trylock 代替lock，
// ,没有枷锁成功的线程不会阻塞，
// 会返回
//
// `pthread_mutex_init'
// `pthread_mutex_destory'
//
// `pthread_mutex_lock'
// `pthread_mutex_trylock'
// `pthread_mutex_timedlock'
//   返回EBUSY 说明mutex 被其他线程加锁了
// `pthread_mutex_unlock'

struct shareObj{
	int a ;
	int b;
} shar = {0, 0};

//static initializtion
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

void up() {
	//enter mutex
	
	int err ;
	if ((err = pthread_mutex_trylock(&mut)) != 0)
		printf("%s\n", strerror(err));
	++shar.a;
	++shar.b;
	sleep(1);
	pthread_mutex_unlock(&mut);
	//leave mutex
	
	printf("thread:%u up a:%d,  b:%d\n", (int)pthread_self(), shar.a, shar.b);
}


void *ct(void *arg) {
	for (int i =1; i< 20; ++i) {
		up();
	}

	return NULL;
}
	int
main ( int argc, char *argv[] )
{
	int err ;
	pthread_t  th1;

	///thread refree a not get mutex is ok
//	for (int i = 5; i > 0; --i) {
//		PCHECK(pthread_mutex_unlock(&mut));
//		sleep(2);
//	}
//	pthread_mutex_t mm ;
//	pthread_mutex_init(&mm)
	err = pthread_create(&th1, NULL, ct, NULL);
	if (err != 0)
		errsys_exit("pthread_create");

	ct(NULL);
	pthread_join(th1, NULL);

	return 0;
}

