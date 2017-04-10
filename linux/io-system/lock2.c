/*
 * =====================================================================================
 *
 *       Filename:  lock2.c
 *
 *    Description:   
 *
 *        Version:  1.0
 *        Created:  03/19/2011 08:56:02 PM
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
#include	<fcntl.h>

static const char *lock_file = "/tmp/LCK.test2" ;

int main() {
	int fp ;

	int tries = 10 ;

	while(tries--) {
	    fp = open(lock_file, O_RDWR | O_CREAT | O_EXCL, 0444) ;
		if(-1 == fp) {
			printf("%d - Lock already present\n", getpid()) ;
			sleep(2) ;
		} else {
			printf("%d - I have exclusive access\n", getpid()) ;
			sleep(1) ;
			close(fp) ;
			unlink(lock_file) ;
			sleep(2) ;
		}
	}
	return 0 ;
}
