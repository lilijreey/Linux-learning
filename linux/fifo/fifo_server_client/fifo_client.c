/*
 * =====================================================================================
 *
 *       Filename:  fifo_client.c
 *
 *    Description:  sent some data to server and receive that
 *					the server write back data print data.
 *
 *        Version:  1.0
 *        Created:  04/09/2011 05:35:50 PM
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
	data_node cli_data ;
	int ser_df, cli_df ;
	char client_fifo_name[20] ;


	//check if have server fifo.
	if(-1 == access(SERVER_FIFO_NAME, F_OK)) {
		printf("Not find server fifo. exit\n") ;
		return 0 ;
	} else {
		cli_data.client_pid = getpid() ;
		sprintf(cli_data.some_data, "Hi I'm %d", getpid()) ;
		//create client fifo and open it
		sprintf(client_fifo_name, CLIENT_FIFO_NAME, getpid()) ;
		E_TEST(-1, mkfifo(client_fifo_name, 0777))

		//write data to server
		EV_TEST(-1, ser_df, open(SERVER_FIFO_NAME, O_WRONLY))
		E_TEST(-1, write(ser_df, &cli_data, sizeof(data_node)))
		printf("%s\n", cli_data.some_data) ;
		S_TEST(0, close(ser_df))

					
		EV_TEST(-1, cli_df, open(client_fifo_name, O_RDONLY))
		printf("client fifo opened\n") ;

		E_TEST(-1, read(cli_df, &cli_data, sizeof(data_node)))
		printf("%s\n", cli_data.some_data) ;
		
		S_TEST(0, close(cli_df))
		//delete client fifo
		S_TEST(0, unlink(client_fifo_name))
	}
	return 0 ;
}
