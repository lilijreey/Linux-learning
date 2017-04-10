/*
 * =====================================================================================
 *
 *       Filename:  pipe1.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/06/2011 10:38:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<unistd.h>
#include	<stdio.h>
#include	<string.h>
#include	<sys/wait.h>
#include	<sys/types.h>
#include	<signal.h>

#include	"myUtility.h"

static void sigpipe_handle(int signo) 
{
	printf("call sigpipe_handle, signo:%d\n", signo) ;
}

int main()
{
	(void)signal(SIGPIPE, sigpipe_handle) ;
	int data_processed ;
	int pipefd[2] ;
	const char some_data[] = "Hello child" ;
	char buf[BUFSIZ + 1] ;

	memset(buf, 0, sizeof(buf)) ;
	E_TEST(-1, pipe(pipefd))

#if 0
	/* close wirte end, then read return 0 */
	printf("got a pipe[0]:%d, [1]:%d\n", pipefd[0], pipefd[1]) ;
	close(pipefd[1]) ;
	int rev		= read(pipefd[0], buf, sizeof(buf)) ;
	printf("read size :%d\n", rev) ;
#endif 

	/* close read then, write, kernel send SIGPIPE, socket is same */
	printf("fork a new process\n") ;
	close(pipefd[0]) ;
	int rev = write(pipefd[1], some_data, sizeof(some_data)) ; 
	if (rev < 0) {
		if (rev == EPIPE)
			printf("write errno is EPIPE") ;
		else 
			perror("write") ;

		exit(1) ;
	}
	printf("read size :%d\n", rev) ;

//	int cpid = fork() ;
//	if(0 == cpid) {//child process
//		write(pipefd[1], some_data, sizeof(some_data)) ; 
//		read(pipefd[0], buf, sizeof(buf)) ;
//		printf("from child read:%s\n", buf) ;
//		return 0 ;
//	} else {
//		sleep(1) ;
//		read(pipefd[0], buf, sizeof(buf)) ;
//		printf("from parent read:%s\n", buf) ;
//		wait(NULL) ; //wait for child
//	}
	

	return 0 ;
}
