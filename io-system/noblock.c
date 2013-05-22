/*
 * =====================================================================================
 *
 *       Filename:  14-1-noblock.c
 *
 *    Description:  
 *
    if (blocking)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;

 *        Created:  12/26/2011 11:49:24 PM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */
#include	<stdio.h>
#include	<fcntl.h>
#include	<errno.h>
#include	<unistd.h>
#include	<zhao/tools.h>

char buf[50000] ;

int main() {

	int n2write, nwrite;
	char *ptr;
		int		val;

	n2write = read(STDIN_FILENO, buf, sizeof(buf));
	fprintf(stderr, "Raed %d bytes\n", n2write) ;


	if ((val = fcntl(STDOUT_FILENO, F_GETFL, 0)) < 0)
		perror("fcntl F_GETFL error");

	val |= O_NONBLOCK;		/* turn on flags */

	if (fcntl(STDOUT_FILENO, F_SETFL, val) < 0)
		perror("fcntl F_SETFL error");

	ptr = buf;
	while (n2write > 0) {
		errno=0;
		nwrite = write(STDOUT_FILENO, ptr, n2write) ;
		fprintf(stderr, "nwrite = %d, errno=%d\n", nwrite, errno);

		if (nwrite > 0) {
			ptr += nwrite ;
			n2write -= nwrite ;
		}
	}

	if ((val = fcntl(STDOUT_FILENO, F_GETFL, 0)) < 0)
		perror("fcntl F_GETFL error");

	val &= ~O_NONBLOCK;		/* turn flags off */

	if (fcntl(STDOUT_FILENO, F_SETFL, val) < 0)
		perror("fcntl F_SETFL error");

	return 0 ;
}

