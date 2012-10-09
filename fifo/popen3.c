/*
 * =====================================================================================
 *
 *       Filename:  popen3.c
 *
 *    Description:  pipe from ps -ax
 *
 *        Version:  1.0
 *        Created:  04/06/2011 10:01:59 PM
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
#include	"myUtility.h"

#define		BUF_SIZE  1024 

int main()
{
	FILE *pipe_in = NULL ;
	char buf[BUF_SIZE+1] ;

	memset(buf, 0, sizeof(buf)) ;

	EV_TEST(NULL, pipe_in, popen("ps ax", "r"))
	int read_chars = fread(buf, sizeof(char), BUF_SIZE, pipe_in) ;  
	while(read_chars > 0) {
		fputs("\n**************new read**************\n", stdout) ;
		fwrite(buf, sizeof(char), read_chars, stdout) ;
		read_chars = fread(buf, sizeof(char), BUF_SIZE, pipe_in) ;  
	}
	
	E_TEST(-1, pclose(pipe_in))
	
	return 0 ;
}
