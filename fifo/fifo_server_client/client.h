/*
 * =====================================================================================
 *
 *       Filename:  client.h
 *
 *    Description:  use to sever and client source file
 *
 *        Version:  1.0
 *        Created:  04/09/2011 04:04:05 PM
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
#include	<string.h>
#include	<limits.h>
#include	"myUtility.h"



#define	SERVER_FIFO_NAME "./serv_fifo"			/*  */
#define CLIENT_FIFO_NAME "./cli%d_fifo"

#define BUFFER_SIZE 50 

typedef struct data_to_pass_st {
	pid_t client_pid ;
	char some_data[BUFFER_SIZE -1] ;
}data_node ;

