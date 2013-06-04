/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  ld d1 d2
 *					全局符号介入
 *
 *        Created:  02/21/2012 08:44:18 AM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */
#include	<stdio.h>
//#include	<unistd.h>
//#include	<stdlib.h>
//#include	<string.h>
//#include	<zhao/tools.h>

void b1();
void b2();
int main(int argc, char *argv[]) {

	b1();
	b2();
	return 0 ;
}

