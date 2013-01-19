/*
 * =====================================================================================
 *
 *       Filename:  lifecycle.c
 *
 *    Description:  
 *
 *        Created:  12/11/2011 11:48:05 PM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */
#include	<stdio.h>
#include	<pthread.h>
#include	<unistd.h>
//#include	<stdlib.h>
//#include	<string.h>
#include	<zhao/tools.h>

void* thread_routine(void *arg) {
	(void)sleep(2) ;
	return arg ;
}

int main() {
	pthread_t thread_id = 0 ;
	void *thread_result = NULL;
	int status = 0;

	status = pthread_create(
			&thread_id, NULL, thread_routine, NULL) ;
	if (0 != status) 
		errno_msg(status, "pthread_create error") ;

	status = pthread_join(thread_id, &thread_result);
	if (0 != status) 
		errno_msg(status, "pthread_join error") ;

	if (thread_result == (void*)NULL) {
		printf("thread return nULL\n") ;
		return 0 ;
	}
	else 
		return 1 ;
}

