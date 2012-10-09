/*
 * =====================================================================================
 *
 *       Filename:  exec_calls.c
 *
 *    Description:  use execl(), execlp(), execle(), 
 *					execv(),  execvp(), execve()
 *
 *        Version:  1.0
 *        Created:  03/27/2011 08:48:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#include	<stdio.h>
#include	<unistd.h>
#include	<sys/types.h>

static char *const ps_argv[] = {"ps", "ax", 0} ;

static char *const ps_envp[] = {"PATH=/bin:/usr/bin", "TERM=console", 0} ;

/*
 * execl("/bin/ps", "ps", "ax", 0)
 * execp("ps", "ps", "ax", 0) //the second ps is what
 * execle("/bin/ps", "ps", "ax", 0, ps_envp)
 * 
 * execv("bin/ps", ps_argv)
 * execvp("ps", ps_argv)
 * execve("bin/ps", ps_argv, ps_envp)
 */

void execlp_call(void)
{
	printf("Running ps with execlp\n") ;
	execlp("ps", "ax", (char *)0) ;
	printf("Done.\n") ;
}

void fork_call(void)
{
	pid_t child_pid;

	printf("fork program starting\n") ;
	child_pid = fork() ;
	switch ( child_pid ) {
		case 0:	
			printf("this is a new process\n") ;
			break;

		case -1:
			perror("fork") ;
			break;

		default:	
			printf("this is father process.create child pid is %d\n", child_pid) ; 
			break;
	}				/* -----  end switch  ----- */
}

int main()
{
	fork_call()  ;
	return 0 ;
}

