/*
 * =====================================================================================
 *
 *       Filename:  wait-about.c
 *
 *    Description:  
 *
 *        Created:  12/10/2011 11:44:18 AM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */
#include	<stdio.h>

#include	<sys/wait.h>
//#include	<unistd.h>
//#include	<stdlib.h>
//#include	<string.h>
//#include	<zhao/tools.h>

void 
pr_exit(int status) {
	if (WIFEXITED(status))
		printf("normal termination, exit status:%d\n", 
				WEXITSTATUS(status)) ;
	else if (WIFSIGNALED(status))
		printf("abnormal termination, exit singal:%d,%s\n", 
				WTERMSIG(status)
#ifdef WCOREDUMP
				WCOREDUMP(status) ? " (core file generated)" : "") ;
#else
				"") ;
#endif
	else if (WIFSTOPPED(status))
		printf("child stoped, signal :%d\n", WSTOPSIG(status)) ; 
	else 
		prinft("child status no recgnoaed") ;
}
int main() {

	return 0 ;
}

