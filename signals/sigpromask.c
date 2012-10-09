/*
 * =====================================================================================
 *
 *       Filename:  sigpromask.c
 *
 *    Description:  signal mask op
 *
 *        Version:  1.0
 *        Created:  10/17/2011 08:28:22 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<signal.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<errno.h>
#include <zhao/tools.h>

void pr_mask() {
	sigset_t sigset ;
    // 默认signal mask为空
	if (sigprocmask(0, NULL, &sigset) < 0) 
		perror("sigprocmask error") ;


	if (sigismember(&sigset, SIGINT)) printf("SIGINT") ;
	if (sigismember(&sigset, SIGUSR1)) printf("SIGUSR1") ;
	if (sigismember(&sigset, SIGALRM)) printf("SIGALRM") ;


    //EE fillset 包括所有signal kill， stop
    E_TEST(-1, sigfillset(&sigset));
    if (sigismember(&sigset, SIGKILL) == 1)
        printf("SIGKILL is in fill set\n");
    else
        printf("SIGKILL not in fill set\n");
    
    //EE 解除一个没有被block的信号,不会引起错误
    E_TEST(-1, sigprocmask(SIG_UNBLOCK, &sigset, NULL));
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{
	pr_mask();
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
