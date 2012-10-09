/*
 * =====================================================================================
 *
 *       Filename:  test-select.c
 *
 *    Description:  
 *
 *        Created:  12/28/2011 10:14:36 PM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */
#include	<stdio.h>
#include	<sys/select.h>

#include	<unistd.h>
//#include	<stdlib.h>
//#include	<string.h>
#include	<zhao/tools.h>

#define	TIMEOUT 2			/*  */
#define	BUF_LEN 1024		/*  */
int main() {

	struct timeval tv ;
	fd_set readfds ;
	int ret ;

	FD_ZERO(&readfds) ;
	FD_SET(STDIN_FILENO, &readfds) ;

	tv.tv_sec = TIMEOUT ;
	tv.tv_usec = 0 ;

	ret = select(1,  // 以这个参赛为监听的最大fd
			&readfds, 
			NULL, 
			NULL, 
			&tv);
	if (ret == -1) {
		perror("select");
		return 1;
	} else if (ret == 1) {
		printf("%d seconds elapsed.\n", TIMEOUT) ;
		return 0;
	}

	printf("select out\n");
	if (FD_ISSET(STDIN_FILENO, &readfds)) {
		char buf[BUF_LEN+1] ;
		int len ;

		EV_TEST(-1, len, read(STDIN_FILENO, buf, BUF_LEN)) ;
		if (len != 0) {
			buf[len]='\0';
			printf("read:%s\n", buf) ;
		}
		return 0 ;
	}
	fprintf(stderr, "This should not happend\n") ;

	return 1 ;
}

