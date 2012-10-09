/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/05/2011 10:30:17 PM
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

/* thses functions use to atexit register */
#if 0
void re_1() {
	printf("re_1\n") ;
}

void re_2() {
	printf("re_2\n") ;
}

void re_3() {
	printf("re_3\n") ;
}

int main () {
	atexit(re_1) ;
	atexit(re_2) ;
	atexit(re_3) ;

	return 0 ;
}
#endif

/*
 * show environ table
 * if you want to list environ-list use envirson variable
 * getenv(), setenv() ;
 */
extern char **environ ;
int main() {
char **en = NULL ; 
	for (en = environ; *en != NULL; *en++) {
		printf("%s\n", *en) ;
	}

	return 0 ;
}


/*
 * =====================================================================================
 *
 *       Filename:  environ.c
 *
 *    Description:  环境对程序的作用？
 *    环境的相关函数，环境是如何传递给程序的？
 *
 *        Created:  01/03/2012 10:04:34 AM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */
#include	<stdio.h>
#include	<unistd.h>
#include	<stdlib.h>
//#include	<string.h>
//#include	<zhao/tools.h>
//
//NOTE: 使用environ变量需要声明一下
//fork: 后父子进程的env一样
extern char **environ ;

int main() {
	int i;
	for (i = 0; environ[i]; ++i) {
		printf("%s\n", environ[i]) ;
	}

	return 0 ;
}

