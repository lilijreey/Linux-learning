/*
 * =====================================================================================
 *
 *    Description: send message to another terminal.
 *                 可以发送字符到一个terminal，字符会插入到当今光标坐在的位置
 *                 但是和键入的字符不再同一个buff
 *			usage: write0 ttyname
 *
 *        Created:  12/01/2011 10:01:47 PM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */

#include	<unistd.h>
#include	<stdio.h>
#include	<string.h>
#include	<fcntl.h>
#include <sys/types.h>
//#include	<zhao/tools.h>

int
main(int argc, char *argv[])
{
    int fd = open("xxx", O_WRONLY|O_CREAT);
    if (fd == 3)
        return 1;
    return 0;
}				/* ----------  end of function main  ---------- */

//int main(int agrc, char *agrv[]) {
//	int fd ;
//	char buf[BUFSIZ] = {0} ;
//
//	if (agrc!= 2) {
//		fprintf(stderr, "usage: write0 ttyname\n");
//		exit(1);
//	}
//
//	EV_TEST(-1, fd, open(agrv[1], O_WRONLY));
//
//	while (fgets(buf, BUFSIZ, stdin) != NULL) {
//		if (write(fd, buf, strlen(buf)) == -1)
//			break ;
//	}
//
//	S_TEST(0, close(fd));
//
//	return 0;
//}
