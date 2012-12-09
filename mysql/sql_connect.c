/*
 * =====================================================================================
 *
 *       Filename:  sql_connect.c
 *
 *    Description:  use mysql api connects database
 *
 *        Version:  1.0
 *        Created:  03/22/2011 08:15:11 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */
/*  gcc -Wall sql_connect.c -o sql_connect -I/usr/include/mysql -lmysqlclient */

#include	<stdio.h>
#include	<stdlib.h>
#include	"mysql.h"
#include	"myUtility.h"


int main()
{
	/*  compare under two block 
	MYSQL *connect_ptr = NULL ;
	//init MYSQL struct
	TFUNVR(NULL, connect_ptr, mysql_init(NULL))
	
	//connect to mysql 
	TFUNR(NULL, mysql_real_connect(connect_ptr,"localhost", "god", "12345", "cake", 0, NULL, 0))
	mysql_close(connect_ptr) ;
	return 0 ;
	*/


	MYSQL sql_conn ; //It's a struct not a pointer
	TFUNR(NULL, mysql_init(&sql_conn))
	
	//connects mysql database
	if(NULL == mysql_real_connect(&sql_conn, 
				"localhost", "god", "12345", "cake", 0, NULL, 0)) {

		fprintf(stderr, "Connection failed\n") ;
		if(mysql_errno(&sql_conn)) {
			fprintf(stderr, "Connection error %d: %s\n", 
					mysql_errno(&sql_conn), mysql_error(&sql_conn)) ;
		}
	} else {
		printf("mysql database connection success.\n") ;
	}

	mysql_close(&sql_conn) ;
	return 0 ;
}
