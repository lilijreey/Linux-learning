/*
 * =====================================================================================
 *
 *       Filename:  signal.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/20/2011 08:21:42 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/types.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<signal.h>
#include <strings.h>

const char* const signame[] = {
	"SIG_0",
	"SIGHUP",	"SIGINT",	"SIGQUIT",	"SIGILL",
	"SIGTRAP",	"SIGABRT",	"SIGBUS",	"SIGFPE",
	"SIGKILL",	"SIGUSR1",	"SIGSEGV",	"SIGUSR2",
	"SIGPIPE",	"SIGALRM",	"SIGTERM",	"SIGSTKFLT",
	"SIGCHLD",	"SIGCONT",	"SIGSTOP",	"SIGTSTP",
	"SIGTTIN",	"SIGTTOU",	"SIGURG",	"SIGXCPU",
	"SIGXFSZ",	"SIGVTALRM","SIGPROF",	"SIGWINCH",
	"SIGIO",	"SIGPWR",	"SIGSYS",
	// read time system signal by follow
	"SIGRT_0",	"SIGRT_1",	"SIGRT_2",	"SIGRT_3",
	"SIGRT_4",	"SIGRT_5",	"SIGRT_6",	"SIGRT_7",
	"SIGRT_8",	"SIGRT_9",	"SIGRT_10",	"SIGRT_11",
	"SIGRT_12",	"SIGRT_13",	"SIGRT_14",	"SIGRT_15",
	"SIGRT_16",	"SIGRT_17",	"SIGRT_18",	"SIGRT_19",
	"SIGRT_20",	"SIGRT_21",	"SIGRT_22",	"SIGRT_23",
	"SIGRT_24",	"SIGRT_25",	"SIGRT_26",	"SIGRT_27",
	"SIGRT_28",	"SIGRT_29",	"SIGRT_30",	"SIGRT_31",
	"SIGRT_32"
};

//	if((rev = sigismember(&sset, SIGCHLD)) == -1) {
//		perror("sigismember error") ;
//		exit(1) ;
//	}else {
//		printf("SIGCHLD is %s in sigset", rev ? "" : "not") ;
//	}
static void siginit_handle(int signo) 
{
	printf("call siginit_handle, signo:%s\n", signame[signo] ) ;

}

static void sigalrm_handle(int signo) 
{
	printf("%u call sigalrm_handle, signo:%s\n", getpid(), signame[signo]) ;
}

static void sigterm_handle(int signo) 
{
	printf(":%u call sigterm_handle, signo:%s\n",  getpid(), signame[signo] ) ;
}

static void sigquit_handle(int signo) 
{
	printf(":%u call sigterm_handle, signo:%s\n",  getpid(), signame[signo] ) ;
	if (SIG_DFL == signal(SIGQUIT, SIG_DFL)) 
	{
		printf("old quit handle is SIG_DFL");

	}
}

static void sigchld_handle(int signo) {
	int pid ;
	printf("call sigchld_handle, signo:%s\n", signame[signo] ) ;
	if ((pid = waitpid(-1, NULL, WNOHANG|__WALL)) < 0) {
		perror("waitpid eror") ;
		exit(1);
	}else {
		printf("termianl child pid:%d\n", pid) ;
	}
}


	//SIGCHLD只有child结束时才会产生，fork时不会产生
static void init_signal() {
//	E_TEST(SIG_ERR , signal(SIGALRM, sigalrm_handle)) ;
//	E_TEST(SIG_ERR , signal(SIGCHLD, sigchld_handle)) ;
//	E_TEST(SIG_ERR , signal(SIGINT, siginit_handle)) ;
//	E_TEST(SIG_ERR , signal(SIGTERM, sigterm_handle)) ;
//	E_TEST(SIG_ERR , signal(SIGQUIT, sigquit_handle)) ;

//

//	E_TEST(-1, sigaddset(&sset, SIGCHLD)) ;

	//SETMASK = clear oldsigset, SIG_BLOCK &sset
//	E_TEST(-1, sigprocmask(SIG_SETMASK, &sset, NULL)) ;
}
int main()
{
	//signal 可以打断sleep，
	// 主进程给自己发singal，子进程会收到吗。
	// 不会

	//fork child
#if 0
	int pid  ;
	pid = fork() ;
	if (pid < 0) {
		perror("fork error");
		exit(1) ;
	} else if (pid == 0) {
		init_signal() ;
		printf("child process start\n") ;

		for (;;) {
			printf("child %d procedd runing\n", getpid());

		sleep(2);
		}
		printf("child process over\n") ;
	} else {
		init_signal() ;
		printf("perant pid:%d\n", getpid()) ;
		sleep(1);
		for (;;) {

			//	kill(getpid(), SIGALRM);
				sleep(1);
		}
	}
#endif

	//block sigalrm
#if 0
	sigset_t sset ;
	E_TEST(-1, sigemptyset(&sset)) ;
	E_TEST(-1, sigaddset(&sset, SIGALRM)) ;
	E_TEST(-1, sigprocmask(SIG_BLOCK, &sset, NULL)) ;
	int ar ;
	for (ar =0; ar < 5; ++ar)
		alarm(2) ;

	//nonblock sigalrm
	E_TEST(-1, sigprocmask(SIG_UNBLOCK, &sset, NULL)) ;
	sleep(4) ;

#endif

    
    // set different siganl with one sigaction
    // ok
	struct sigaction sa;
    bzero(&sa, sizeof(sa));

	sa.sa_handler = sigalrm_handle;
	sigaction(SIGALRM, &sa, NULL);
	sa.sa_handler = sigquit_handle;
	sigaction(SIGQUIT, &sa, NULL);
	sa.sa_handler = siginit_handle;
	sigaction(SIGINT, &sa, NULL);

    for (;;) {
        pause();
    }


	return 0 ;


}
