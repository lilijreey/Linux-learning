/*
 * =====================================================================================
 *
 *       Filename:  fifo_read.c
 *
 *    Description:  read data from fifo
 *
 *        Version:  1.0
 *        Created:  04/09/2011 02:48:59 PM
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
#include	<limits.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	"myUtility.h"

#define		TEN_MEG (1024 * 1024 * 1024 * 10)
#define		BUF_SIZE PIPE_BUF
int main()
{
	const char fifo_path[] = "./my_fifo" ;
	int re_df ;
	int res = 0 , total = 0 ;
	char buf[BUF_SIZE] ;

	//open fifo by read
	EV_TEST(-1, re_df, open(fifo_path, O_RDONLY))
	
	EV_TEST(-1, res, read(re_df, buf, BUF_SIZE))
	while(res != 0) {
		total += res ;
		printf("Process %d, read %d bytes data\n", getpid(), res) ;
		EV_TEST(-1, res, read(re_df, buf, BUF_SIZE))
	}
	printf("the Process %d, read finished. total read %dkB(s).\n", getpid(), total/1024) ;
	
	return 0 ;
}
