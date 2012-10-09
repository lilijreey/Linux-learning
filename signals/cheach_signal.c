/*
 * =====================================================================================
 *
 *       Filename:  cheach_signal.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/11/2011 08:33:18 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */
#include	<stdio.h>
#include	<signal.h>
#include	<unistd.h>
#include	<stdlib.h>
#include <syslog.h>
#include <assert.h>
static void sig_uer(int) ;

int 
main() {
	if (signal(SIGUSR1, sig_uer) == SIG_ERR)
		perror("SIGUER1 error") ;
	if (signal(SIGUSR2, sig_uer) == SIG_ERR)
		perror("SIGUER2 error") ;
	if (signal(SIGPIPE, sig_uer) == SIG_ERR)
		perror("PROF, error") ;
	if (signal(SIGTERM, sig_uer) == SIG_ERR)
		perror("TERM, error") ;
	if (signal(SIGQUIT, sig_uer) == SIG_ERR)
		perror("QUIT, error") ;
	if (signal(SIGPOLL, sig_uer) == SIG_ERR)
		perror("SIGPOLL, error") ;
	if (signal(SIGHUP, sig_uer) == SIG_ERR)
		perror("SIGHUP, error") ;
	if (signal(SIGINT, sig_uer) == SIG_ERR)
		perror("SIGINT, error") ;
	if (signal(SIGABRT, sig_uer) != SIG_IGN)
		perror("SIGABRT, error") ;
	/* ignor a signal */
	if (signal(SIGPWR, SIG_IGN) != SIG_IGN)
		perror("SIGPOLL, error") ;
		
    // just send sigabrt to process
//    assert(1 < 4 && "fir");
//    assert(1 < 4 && "sec");
    abort();
	for (;;) 
        pause();

	return 0 ;
}

static void
sig_uer(int signo) {

	switch (signo) 
	{
		case SIGUSR1:
			printf("received SIGUSER1\n") ;
			break ;
		case SIGUSR2:
			printf("received SIGUSER2\n") ;
			break ;
		case SIGTERM:
			printf("received SIGTERM\n") ;
			break ;
		case SIGPOLL:
			printf("received SIGPOF\n") ;
			break ;
		case SIGPIPE:
			printf("received SIGPIPE\n") ;
			break ;
		case SIGPWR:
			printf("received SIGPWR\n") ;
			break ;
		case SIGQUIT:
			printf("received SIGQUIT\n") ;
			break ;
        case SIGINT:
			printf("received SIGINT\n") ;
			break ;
		case SIGHUP: // tty关闭时发出
            syslog(LOG_INFO, "evan resave a SIGHUP");
			break ;
		case SIGABRT: // tty关闭时发出
            syslog(LOG_INFO, "evan resave a SIGABRT");
			break ;
		default:
			printf("received nocatch signal %d\n", signo) ;
	}
}

