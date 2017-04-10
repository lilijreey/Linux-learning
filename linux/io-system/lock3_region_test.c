/*
 * =====================================================================================
 *
 *       Filename:  lock3_region_test.c
 *
 *    Description:  test lock3_region.c
 *
 *        Version:  1.0
 *        Created:  03/20/2011 02:52:26 PM
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

void show_lock_info(struct flock *lock)
{
	printf("\tl_type %d,  ",  lock->l_type) ;
	printf("l_whence %d,  ",  lock->l_whence) ;
	printf("l_start %ld,  ",  lock->l_start) ;
	printf("l_len %ld,  ",  lock->l_len) ;
	printf("l_pid %d\n",  lock->l_pid) ;
}

int main() 
{
	const char *test_file = "/tmp/test_lockd" ;
	int fp ;
	const int SIZE_TO_TRY = 5 ;
	struct flock tflock ;

	//open file 
	TFUNVR(-1, fp, open(test_file, O_RDWR | O_CREAT, 0666))

	printf("F_WRLCK=%d F_RDLOCK=%d F_UNLCK=%d\n", F_WRLCK, F_RDLCK, F_UNLCK) ;
	for(int start_byte = 0; start_byte < 99; start_byte += SIZE_TO_TRY) {
		//test flock
		int res ;

		tflock.l_type = F_RDLCK;
		tflock.l_whence = SEEK_SET ;
		tflock.l_start = start_byte ;
		tflock.l_len = SIZE_TO_TRY ;
		tflock.l_pid = getpid() ;

		printf("Testing F_WRLCK on region from %d to %d\n", 
				start_byte,  start_byte + SIZE_TO_TRY) ;

		TFUNVR(-1, res, fcntl(fp, F_GETLK, &tflock))
		if(tflock.l_pid !=  getpid()) {
			printf("\tdifferent from lock_type.F_GETLK returned:\n") ;
			show_lock_info(&tflock) ;
		} else {
			printf("\tsame as lock_type, or no lock.\n") ;
			show_lock_info(&tflock) ;
		}

	   
		//now repeat the test with a shared lock, test again
		tflock.l_type = F_RDLCK ;
		tflock.l_whence = SEEK_SET ;
		tflock.l_start = start_byte ;
		tflock.l_len = SIZE_TO_TRY ;
		tflock.l_pid = getpid() ;
	printf("Testing F_RDLCK on region from %d to %d\n", 
				start_byte,  start_byte+SIZE_TO_TRY) ;
 
		TFUNVR(-1, res, fcntl(fp, F_GETLK, &tflock))
		if(tflock.l_pid !=  getpid()) {
			printf("\tdifferent from lock_type.F_GETLK returned:\n") ;
			show_lock_info(&tflock) ;
		} else {
			printf("\tsame as lock_type, or no lock.\n") ;
			show_lock_info(&tflock) ;
		}
	}

	TFUNR(-1, fp)
	return 0 ;
}
