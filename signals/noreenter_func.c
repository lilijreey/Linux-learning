/*
 * =====================================================================================
 *
 *       Filename:  noreenter_func.c
 *
 *    Description:  10-2
 *
 *        Version:  1.0
 *        Created:  10/12/2011 07:27:02 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<pwd.h>
#include	<stdio.h>
#include	<string.h>
#include	<unistd.h>
#include	<signal.h>

static void 
my_alarm(int signo) {
	struct passwd *rootptr ;
	printf("in signal handler\n") ;
	if ((rootptr = getpwnam("root")) == NULL)
		perror("getpwname error") ;
	alarm(1) ; /* send sigala once a second */
}

int main() {
	struct passwd *ptr ;

	signal(SIGALRM, my_alarm) ;
	alarm(1) ;
	for (;;) {
		if ((ptr = getpwnam("sar")) == NULL)
			perror("getpwnam error in mian") ;
		if (strcmp(ptr->pw_name, "sar") != 0)
			printf("return value corrupted, pw_name = %s\n", ptr->pw_name) ;
	}
	return 0 ;

}
