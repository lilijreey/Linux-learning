/*
 * =====================================================================================
 *
 *       Filename:  pip_writer.c
 *
 *    Description:  this programe create a child process wait read data from
 *					this programe pipe. this progream write data to pipe.
 *
 *        Version:  1.0
 *        Created:  04/07/2011 09:11:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#include	<unistd.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	"myUtility.h"

int main()
{
	const char str[] = "Hi, my child" ;
	int pipedf[2] ;
	int pid = -1 ;
	char buf[1024] ;

	memset(buf, 0, sizeof(buf)) ;

	//get pipe
	E_TEST(-1, pipe(pipedf))
	
	//create a child process
	EV_TEST(-1, pid, fork())
	if(0 == pid) {
		//child process
		sprintf(buf, "%d", pipedf[0]) ;
		E_TEST(-1, execl("pip_read", "pip_read", buf, (char *)0))
		return 0 ;

	} else {
		//parent process write data to pipe
		write(pipedf[1], str, sizeof(str)) ;
		S_TEST(0, close(pipedf[0]))
		S_TEST(0, close(pipedf[1]))	
	}
	return 0 ;
}

