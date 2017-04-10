/*
 * =====================================================================================
 *
 *       Filename:  dup1.c
 *
 *    Description:  use dup create descriptor
 *
 *        Version:  1.0
 *        Created:  04/27/2011 10:36:07 AM
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
#include	<stdlib.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
	int
main (void)
{
	int df ;
	for(;;) {
		if(-1 == (df = dup(1))) {
			perror("dup") ;
			exit(1) ;
		}
		printf("111:create a descriptor=%-2d\n", df) ;
		sleep(1) ;
	}
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
