/*
 * =====================================================================================
 *
 *       Filename:  create_thread.c
 *
 *    Description:  
 *
 *        Created:  12/07/2011 11:15:53 PM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */

#include  <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include	<sys/types.h>
#include	<pthread.h>

#include	<stdio.h>
#include	<stdlib.h>

#include	<zhao/tools.h>
void
printids(int num)
{
	pid_t pid ;
	pthread_t tid ;

	pid = getpid() ;
	tid = pthread_self() ;
	printf("%d pid %u tid %lu (0x%x)\n", num,  pid, tid, (unsigned int)tid) ;
}

//void *thr_fn(void *arg) {
//	printids("new thread: ") ;
//	return ((void *)0) ;
//}

void *openfd(void *arg) {
	int * fd =(int *)arg;

	*fd  = open("create_thread.c", O_RDONLY);
	if (*fd == -1) 
		errsys_exit("open");
	printids(*(int*)arg);
	return ((void *)0) ;
}

void *closefd(void *arg) {
	int *fd = arg ;
	if (close(*fd) == -1)
		errsys_exit("close");
	printids(*(int*)arg);
	return ((void *)0) ;
}


#define	COUNT 4			/*  */
/* use mutex */

struct shareObj{
	int a ;
	int b;
} shar = {0, 0};

	int
main ( int argc, char *argv[] )
{
	int err ;
	int fd = -1 ;
	pthread_t  ntid[COUNT] ;
	for (int i=0; i< COUNT; i+=2) {
		err = pthread_create(&ntid[i], NULL, openfd, (void*)(&fd)) ;
		if (err != 0)
			errsys_exit("pthread_create");
		pthread_join(ntid[i], NULL);

		err = pthread_create(&ntid[i+1], NULL, closefd, (void*)(&fd)) ;
		if (err != 0)
			errsys_exit("pthread_create");
		pthread_join(ntid[i+1], NULL);
	}
	pthread_exit(NULL);
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
