/*
 * =====================================================================================
 *
 *       Filename:  pip_read.c
 *
 *    Description:  read data from pipe
 *
 *        Version:  1.0
 *        Created:  04/07/2011 09:43:05 PM
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

int main(int argc, char *argv[])
{
	int df = -1 ;
	char buf[1024] ;

	memset(buf, 0, sizeof(buf)) ;

	//get df from argv[1]
	sscanf(argv[1], "%d", &df) ;
	if(-1 == df) {
		printf("argv[1] faild\n") ;
		exit(1) ;
	}

	//read data
	read(df, buf, sizeof(buf)) ;
	
	printf("%s. Hi my dade, thank you.\n", buf) ;
	return 0 ;
}
