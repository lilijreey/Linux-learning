/*
 * =====================================================================================
 *
 *       Filename:  dbm1.c
 *
 *    Description:  use gdbm creat, open, store and fetch it
 *
 *        Version:  1.0
 *        Created:  03/21/2011 11:41:10 PM
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
#include	<unistd.h>
#include	<gdbm-ndbm.h>
#include	<string.h>
#include	<fcntl.h>

#include	"myUtility.h"

int main()
{
	const char *database_name = "/tmp/ndbm" ;
	//create a dbm
	DBM *dbmptr = 0 ;
	TFUNVR(0, dbmptr, dbm_open(database_name, O_RDWR | O_CREAT,  0666))
	
	//append some item to dbm from user
	const int DATA_MAX_LEN = 101 ;
	datum key_datum, data_datum ;
	int key, result ;
	char data[DATA_MAX_LEN] ;

	printf("Enter a database item, form is 'key data', when key is -1 to end.\n") ;
	scanf("%d", &key) ;
	while(-1 != key) {
		key_datum.dptr = (void *)&key ;
		key_datum.dsize = sizeof(key) ;
		
		fgets(data, DATA_MAX_LEN , stdin) ;
		data_datum.dptr = data ;
		data_datum.dsize = strlen(data) ;
		
		do{
		if(1 == (result = dbm_store(dbmptr, key_datum, data_datum, DBM_INSERT))) {
			fprintf(stderr, "dbm_store has been a same key\n") ;
			break ;
		} else if(0 > result) {
			fprintf(stderr, "dbm_store failed\n") ;
			exit(1) ;
		}
		}while(0) ; //use for break can scanf
		scanf("%d", &key) ;
	}
	

	//read data frm dbm
	printf("Now you can search itme, enter a key, when key is -1 to end\n") ;
	scanf("%d", &key) ;
	while(-1 != key) 
	{
		key_datum.dptr = (void *)&key ;
		key_datum.dsize = sizeof(key) ;
		data_datum = dbm_fetch(dbmptr, key_datum) ;
		if(NULL == data_datum.dptr) {
			printf("No has this key item, try again:") ;
		} else {
			printf("key:%-4d:%s\n", key, (char *)data_datum.dptr) ; 
		}
		scanf("%d", &key) ;
	}

	//close dbm
	dbm_close(dbmptr) ;

	return 0 ;
}

