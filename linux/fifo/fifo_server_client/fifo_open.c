/*
 * =====================================================================================
 *
 *       Filename:  fifo_open.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/09/2011 01:35:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	"myUtility.h"


int main()
{
	const char fifo_path[] = "./my_fifo" ;
	int rd, wd ;

	//create a fifo if no have my_fifo
	if(-1 == access(fifo_path, F_OK)) {
		E_TEST(-1, mkfifo(fifo_path, 0777))
	}
	
	//open fifo by read 
	//EV_TEST(-1, rd, open(fifo_path, O_RDONLY | O_NONBLOCK))
	//printf("the fifo read opened\n") ;

	//open fifo by write
	EV_TEST(-1, wd, open(fifo_path, O_WRONLY | O_NONBLOCK))
	printf("the fifo write opened\n") ;
	
	//S_TEST(0, close(rd))
	S_TEST(0, close(wd))

	return 0 ;
}
