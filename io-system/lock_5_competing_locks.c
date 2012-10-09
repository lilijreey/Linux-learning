/*
 * =====================================================================================
 *
 *       Filename:  lock_5_competing_locks.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/20/2011 05:51:10 PM
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
#include	"myUtility.h"

int main()
{
	const char *test_file = "/tmp/test_lockd" ;
	int fp ;
	//open file 
	TFUNVR(-1, fp, open(test_file, O_RDWR | O_CREAT, 0666)) 

	//set read lock from 10 to 15 bytes
	struct flock lock;
	lock.l_type = F_RDLCK ;
	lock.l_whence = SEEK_SET ;
	lock.l_start = 10 ;
	lock.l_len = 5 ;
	

	printf("Process %d,  tying F_RDLCK,  region 10 to 15\n", getpid()) ;
	int res = fcntl(fp,  F_SETLK, &lock) ;
	if(-1 == res) {
		printf("Process %d - failed to lock region\n", getpid()) ;
	} else {
		printf("Process %d - obtained to lock region\n", getpid()) ;
	}

	//unlock file form 10 to 5
	lock.l_type = F_UNLCK ;
	printf("Process %d,  trying F_UNLCK, region 10 to 15\n", getpid()) ;
	res = fcntl(fp, F_SETLK, &lock) ;
	if(-1 == res) {
		printf("Process %d - failed to lock region\n", getpid()) ;
	} else {
		printf("Process %d - obtained to lock region\n", getpid()) ;
	}

	//unlock file region from 0 to 50
	lock.l_start = 0 ;
	lock.l_len = 50 ;
	printf("Process %d,  trying F_UNLCK, region 0 to 50\n", getpid()) ;
	res = fcntl(fp, F_SETLK, &lock) ;
	if(-1 == res) {
		printf("Process %d - failed to lock region\n", getpid()) ;
	} else {
		printf("Process %d - obtained to lock region\n", getpid()) ;
	}

	//to lock the region from bytes 16 to 21 with an exclusive lock
	lock.l_type = F_WRLCK ;
	lock.l_start = 16 ;
	lock.l_len = 5;

	printf("Process %d,  trying F_WRLCK, region 16 to 21\n", getpid()) ;
	res = fcntl(fp, F_SETLK, &lock) ;
	if(-1 == res) {
		printf("Process %d - failed to lock region\n", getpid()) ;
	} else {
		printf("Process %d - obtained to lock region\n", getpid()) ;
	}

	//to lock the region from 40 to 50 bytes with shear lock
	lock.l_type = F_RDLCK ;
	lock.l_start = 40 ;
	lock.l_len = 10 ;
	printf("Process %d,  trying F_RDLCK, region 40 to 50\n", getpid()) ;
	res = fcntl(fp, F_SETLK, &lock) ;
	if(-1 == res) {
		printf("Process %d - failed to lock region\n", getpid()) ;
	} else {
		printf("Process %d - obtained to lock region\n", getpid()) ;
	}

	//to lock the region from bytes 16 to 21 with wait exclusive lock
	lock.l_type = F_WRLCK ;
	lock.l_start = 16 ;
	lock.l_len = 5 ;
	printf("Process %d,  trying F_WRCLK, region 16 to 21\n", getpid()) ;
	res = fcntl(fp, F_SETLKW, &lock) ;
	if(-1 == res) {
		printf("Process %d - failed to lock region\n", getpid()) ;
	} else {
		printf("Process %d - obtained to lock region\n", getpid()) ;
	}
	
	TFUNR(-1, close(fp))
	return 0 ;
}
