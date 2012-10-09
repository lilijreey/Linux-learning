/*
 * =====================================================================================
 *
 *       Filename:  sql_insert.c
 *
 *    Description:  insert items to children
 *
 *        Version:  1.0
 *        Created:  03/22/2011 09:32:23 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	"mysql.h"
#include	"myUtility.h"

int main()
{
	MYSQL sql_conn ; //It's a struct not a pointer
	TFUNR(NULL, mysql_init(&sql_conn))
	
	//connects mysql database
	if(NULL == mysql_real_connect(&sql_conn, 
				"localhost", "god", "12345", "cake", 0, NULL, 0)) {

		fprintf(stderr, "Connection failed\n") ;
		if(mysql_errno(&sql_conn)) {
			fprintf(stderr, "Connection error %d: %s\n", 
					mysql_errno(&sql_conn), mysql_error(&sql_conn)) ;
			return 1 ;
		}
	} else {
		printf("mysql database connection success.\n") ;
	}

	//insert items
	int res ;
	res = mysql_query(&sql_conn, 
			"INSERT INTO children(fname, age) VALUES('Ann', 4)") ;
	if(0 != res) {
		fprintf(stderr, "Insert error %d: %s\n", mysql_errno(&sql_conn),
												 mysql_error(&sql_conn)) ;
		return 1 ;
	} else {
		printf("Inserted %lu row\n", (unsigned long)mysql_affected_rows(&sql_conn)) ;
	}

	res = mysql_query(&sql_conn, 
			"UPDATE children SET AGE = 4 WHERE fname= 'Ann'") ;
	if(0 != res) {
		fprintf(stderr, "Update error %d: %s\n", 
					mysql_errno(&sql_conn), mysql_error(&sql_conn)) ;
		return 1 ;
	} else {
		printf("Update %lu rows\n", (unsigned long)mysql_affected_rows(&sql_conn)) ;
	}
			
	mysql_close(&sql_conn) ;
	return 0 ;

}
