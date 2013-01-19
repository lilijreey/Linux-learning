/*
 * =====================================================================================
 *
 *       Filename:  alarm_for.c
 *
 *    Description:  
 *
 *        Created:  12/10/2011 10:29:57 AM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */
#include	<stdio.h>
#include	<stdbool.h>
#include	<unistd.h>
#include	<sys/wait.h>
#include	<stdlib.h>
//#include	<string.h>
#include	<zhao/tools.h>

int main() {
	pid_t pid ;

	unsigned int s = 0 ;
	while (true) {
		printf("##start fork\n") ;
	for (;s < 10; s+=2) {
		EV_TEST((pid_t)-1, pid, fork()) ;
		if (pid == (pid_t)0) {
			(void)printf("create child:%u\n", (unsigned)getpid()) ;
			(void)sleep(s+2) ;
			printf("over child:%u\n", (unsigned)getpid()) ;
			exit(EXIT_SUCCESS) ;
		} else {
			/* wait for any child */
			do {
				int status ;
				(void)printf("parent wiat:%ld\n", (long int)pid) ;
				pid = waitpid(-1, &status, WNOHANG);
				//pid = wait(&status) ;
				if (-1 == pid) {
					perror("waitpid error") ;
					exit(EXIT_FAILURE) ;
				}
				(void)printf("parent returen wiat:%ld status:%d\n", (long int)pid, status );
			} while (pid != (pid_t)0) ;
		}
	}
	}
	return 0 ;
}

