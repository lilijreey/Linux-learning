/*
 * =====================================================================================
 *
 *       Filename:  alarm_thread.c
 *
 *    Description:  
 *
 *        Created:  12/10/2011 10:27:38 PM
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

typedef struct alarm_tag {
	int sec ;
	char message[64] ;
}alarm_t ;


void *alarm_thread(void *arg) {
	alarm_t *alarm = (alarm_t*) arg ;
	int status ;

	status = pthread_detach(pthread_self()) ;
	if (status != 0) {
		perror("Detach thread");
		abort() ;
	}
	(void)sleep(alarm->sec) ;
	(void)printf("(%d) %s : pid:%u tid:%lu\n", alarm->sec, alarm->message, getpid(), (long unsigned )pthread_self());
	free(alarm) ;
	return NULL ;
}
int main() {
	int status ;
	char line[128] ;
	alarm_t *alarm ;
	pthread_t thread ;

	while(1) {
		printf(">>> ") ;
		if (fgets (line, sizeof(line), stdin) == NULL) exit (0) ;
		if (strlen(line) <= 1) continue ;
		alarm = (alarm_t*)malloc(sizeof(alarm_t)) ;
		if (alarm == NULL)
			exit(1) ;

		if (sscanf(line, "%d %64[^\n]", 
					&alarm->sec, alarm->message) < 2) {
			fprintf(stderr, "bad command\n") ;
			free(alarm) ;
		} else {
			status = pthread_create(
					&thread, NULL, alarm_thread, alarm) ;
			if (status != 0) {
				perror("thread_create failus");
				abort() ;
			}
		}
	}

	return 0 ;
}

