/*
 * =====================================================================================
 *
 *       Filename:  fifo_write.c
 *
 *    Description:  create a data write to fifo
 *
 *        Version:  1.0
 *        Created:  04/09/2011 02:29:51 PM
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

#define		TEN_MEG (1024 * 1024 * 1024)
#define		BUF_SIZE PIPE_BUF
int main()
{
	const char fifo_path[] = "./my_fifo" ;
	int res ;
	int wr_df = -1 ;
	char buf[PIPE_BUF] ;
	int write_bytes ;
	
	//create a fifo if no have my_fifo
	if(-1 == access(fifo_path, F_OK)) {
		E_TEST(-1, mkfifo(fifo_path, 0777))
	}

	EV_TEST(-1, wr_df, open(fifo_path, O_WRONLY))
	printf("Process %d result %d\n", getpid(), wr_df) ;

	
	for(write_bytes = 0 ; write_bytes < TEN_MEG; write_bytes += res) {
		//write to fifo
		EV_TEST(-1, res, write(wr_df, buf, BUF_SIZE))
	}
	printf("the Process %d, total send %dKB(s).\n", getpid(), write_bytes/1024) ;
	
	return 0 ;
}
