/*
 * =====================================================================================
 *
 *       Filename:  lock1.c
 *
 *    Description:  use file locking
 *
 *        Version:  1.0
 *        Created:  03/19/2011 08:35:59 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey , lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<fcntl.h>
#include	<errno.h>

int main(){
	int fp ;

	fp = open("/tmp/LCK.test",  O_RDWR | O_CREAT | O_EXCL,  0444) ;
	if(-1 == fp) {
		perror("open") ;
	} else {
		printf("Open success\n") ;
	}
	return 0 ;	
}
