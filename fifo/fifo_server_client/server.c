/*
 * =====================================================================================
 *
 *       Filename:  server.c
 *
 *    Description:  server receive data from client and send the data of conut 
 *						to client
 *
 *        Version:  1.0
 *        Created:  04/09/2011 04:58:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	"client.h"

int main() 
{
	int server_df = -1, client_df = -1 ;
	struct data_to_pass_st data ;
	char client_fifo_name[250] ;
	char receive_str[20] ;

	//init receive_str
	sprintf(receive_str, "-Server:%d", getpid()) ;


	//create sever fifo
	if(-1 == access(SERVER_FIFO_NAME, F_OK)) {
		E_TEST(-1, mkfifo(SERVER_FIFO_NAME, 0777))
	}

	
	while(1) {
	
		//open server fifo
		EV_TEST(-1, server_df, open(SERVER_FIFO_NAME, O_RDONLY))
		printf("I'm listening...\n") ;
		//read the recevie data from client
		E_TEST(-1, read(server_df, &data, sizeof(data_node)))
		printf("I'm received data\n") ;
	
		//open client fifo
		sprintf(client_fifo_name, CLIENT_FIFO_NAME, data.client_pid) ;	
		EV_TEST(-1, client_df, open(client_fifo_name, O_WRONLY))
		printf("client fifo is opened ") ;

		strcat(data.some_data, receive_str) ; 
		//write back client fifo
		E_TEST(-1, write(client_df, &data, sizeof(data_node)))
		printf("write backed\n") ;

		//close the client fifo
		S_TEST(0, close(client_df))
		S_TEST(0, close(server_df))
	}
	return 0 ;
}
