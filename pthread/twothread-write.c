/*
 * =====================================================================================
 *
 *       Filename:  twothread-write.c
 *
 *    Description:  create two thread is read and print other write
 *
 *        Created:  12/17/2011 08:13:25 PM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */
#include	<stdio.h>
#include	<pthread.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<zhao/tools.h>


//static pthread_mutex_t x = PTHREAD_MUTEX_INITIALIZER ;
void * print_word(void *arg) {
//lock
//	pthread_mutex_trylock(&x) ;


//unlock
//	pthread_mutex_unlock(&x) ;
	sleep(1) ;	
	for (;;) {
		(void)printf("#i:%d :%lu\n",  *(int*)arg, (unsigned long int)pthread_self()) ;
	}
}

#define COUNT  100
int main() {

	pthread_t thread_ids[COUNT] = {0}  ;
	int args[COUNT] ;
	int i = 0 ;
	for (; i < COUNT; ++i) {
		args[i] =i;
	}


	for (i=0; i< COUNT; ++i) {
		S_TEST(0, pthread_create(&thread_ids[i], NULL, print_word, &args[i])) ;
	}

	
	sleep(20) ;
//	100个线程时，10 秒下每个线程的执行概率很不均云, 20second 的时候，各个线程的执行差距在万级别
//	10 个进程时，1秒钟执行的差距在千次级别


	return 0 ;
}

