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
//see man select_tut(2)
//
//1. select 每次loop都要重新设置fd，也就是本次设置的set 在select返回后就没有了
//2. select 返回后并不知道那写fd是set了，必须轮训所有的set中的fd FD_JISSET(
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

