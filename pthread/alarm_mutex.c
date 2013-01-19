/*
 * =====================================================================================
 *
 *       Filename:  alarm_mutex.c
 *
 *    Description:  
 *
 *        Created:  12/12/2011 09:57:41 PM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */
#include	<stdio.h>
#include	<pthread.h>
#include	<time.h>
#include	<stdbool.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<string.h>
#include	<zhao/tools.h>

typedef struct alarm_tag {
	struct alarm_tag *link;
	int    sec ;
	time_t time;
	char   msg[64] ;
} alarm_t ;

static pthread_mutex_t alarm_mutex = PTHREAD_MUTEX_INITIALIZER ;
static alarm_t *alarm_list = /*@null@*/NULL ;

static void* alarm_thread(void *arg) {
	if ((void*)NULL == arg)
		return (void*)NULL ;

	alarm_t *alarm_ptr ;
	int sleep_time ;
	time_t now ;

	while (true) {
		S_TEST(0, pthread_mutex_lock(&alarm_mutex)) ;
		alarm_ptr = alarm_list ;

		if (alarm_ptr == NULL) 
			sleep_time = 1;
		else {
			alarm_list = alarm_ptr->link ;
			now = time(NULL) ;
			if (alarm_ptr->time <= now)
				sleep_time = 0 ;
			else 
				sleep_time = alarm_ptr->time - now ;

			debug_msg(stdout, "[waiting: %d(%d)\"%s\"]\n", (int)(alarm_ptr->time), 
					sleep_time, alarm_ptr->msg) ;
		}

		S_TEST(0, pthread_mutex_unlock(&alarm_mutex));
		if (sleep_time > 0)
			(void)sleep(sleep_time) ;
		else
			(void)sched_yield() ;

		if (alarm_ptr != NULL) {
			(void)printf("(%d)%s\n", alarm_ptr->sec, alarm_ptr->msg) ;
			free(alarm) ;
		}
	}
}


int main() {
	char line[128];
	alarm_t *alarm_ptr, **last, *next ;
	pthread_t thread ;

	S_TEST(0, pthread_create(
				&thread, NULL, alarm_thread, NULL)) ;
	while (true) {
		(void)printf("alarm>>> ") ;
		if (fgets(line, sizeof(line), stdin) == NULL) exit(EXIT_SUCCESS) ;
		if (strlen(line) <= 1) continue ;

		alarm_ptr = (alarm_t*)malloc (sizeof(alarm_t)) ;
		if (NULL == alarm_ptr) 
			err_msg("malloc have a failuer") ;

		if (sscanf(line, "%d %64[^\n]", 
					&alarm_ptr->sec, alarm_ptr->msg) < 2) 
		{
			fprintf(stderr, "Bad command\n") ;
			free(alarm_ptr) ;
		} else {
			S_TEST(0, pthread_mutex_lock(&alarm_mutex)) ;
			alarm_ptr->time = time(NULL) + alarm_ptr->sec ;

			last = &alarm_list ;
			next = *last ;

			while (next != NULL) {
				if (next->time >= alarm_ptr->time) {
					alarm_ptr->link = next ;
					*last = alarm_ptr ;
					break ;
				}
				last = &next->link;
				next = next->link ;
			}


		}

	}

	return 0 ;
}

