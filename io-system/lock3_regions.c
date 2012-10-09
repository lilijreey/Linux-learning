/*
 * =====================================================================================
 *
 *       Filename:  lock3_regions.c
 *
 *    Description:  use the fcntl(int fp,  int command, flock *flock_struct)
 *					create a lock 
 *
 *        Version:  1.0
 *        Created:  03/20/2011 10:37:05 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#define	_GUN_SOURCE			/*  */
#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>
						
/* the mraco test a function if has error reture value, 
 * the error is error value,  rev is the function reture value, 
 * func is you called function, and arguments,  It must be
 *	func(arg, arg)
 * */
#define	TFUNVR(error, rev, func)   if((error) == ((rev) = (func))) {\
										printf("%s|%d|", __FILE__, __LINE__) ;\
										perror(#func) ;\
										exit(1) ;\
								}

/*
 * the mraco test a function if error return value, 
 * but no rev 
 */
#define TFUNR(error, func)		if((error) == (func)) {\
									printf("%s|%d|", __FILE__, __LINE__) ;\
									perror(#func) ;\
									exit(1); \
								}

int main()
{
	const char *test_file = "/tmp/test_lockd" ;
	char *byte_to_write = "A" ;
	int byte_conut ;

	//create temp file and open use to lock file
	int fp ;
	//fp =open(test_file, O_RDWR | O_CREAT, 0666) ;
	TFUNVR(-1, fp, open(test_file, O_RDWR | O_CREAT, 0666)) 
	
	//put some data in the file
	for(byte_conut = 0 ; byte_conut < 100; byte_conut++) {
		write(fp, byte_to_write, 1) ;
	}

	struct flock region_1 , region_2 ;
	int res ;
	//set up region 1 with a shared lock,  from bytes 10 to 30
	region_1.l_type = F_RDLCK ;
	region_1.l_whence = SEEK_SET ;
	region_1.l_start = 10 ;
	region_1.l_len = 20 ;

	//set up region 2 with an exclusive lock,  from by 40 to 50
	region_2.l_type = F_WRLCK ;
	region_2.l_whence = SEEK_SET ;
	region_2.l_start = 40 ;
	region_2.l_len = 10 ;

	//now lock the file
	printf("Process %d locking file\n",  getpid()) ;
	TFUNVR(-1, res, fcntl(fp, F_SETLK, &region_1)) 
	TFUNVR(-1, res, fcntl(fp, F_SETLK, &region_2)) 	
	
	//wait for while
	sleep(10) ;

	//close and delte temp file
	printf("Process %d closing and delete file\n", getpid()) ;
	TFUNR(-1, close(fp)) 
	TFUNR(-1, unlink(test_file))

	return 0 ;
}
