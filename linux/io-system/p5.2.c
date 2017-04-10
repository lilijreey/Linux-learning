/*
 * =====================================================================================
 *
 *       Filename:  p5.2.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/04/2011 09:54:23 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include <stdio.h>

/*
 * 当得到的数据超过buf是并不会错误，
 * 而是分多次盗用fets知道数据读完
 */
#define MAXLINE 4 /* 会保留一个B存放‘/0’，所以buf的大小不能为1 */ 

int main() {
	//4444 4444 4444 4444 4444 4444
	char buf[MAXLINE] ;

	while (fgets(buf, MAXLINE, stdin) != NULL) {
		printf("\n#\n") ; /* 指示循环 */
		if (fputs(buf, stdout) == EOF)
			perror("output error") ;
	}
	if (ferror(stdin))
		perror("input error") ;
	
	return 0 ;
}

void za_setbuf(FILE *stream, char *buf) {
	setvbuf(stream, buf, buf ? _IOFBF : _IONBF, BUFSIZ) ;
}
