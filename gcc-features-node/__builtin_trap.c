/*
 * =====================================================================================
 *
 *       Filename:  __builtin_trap.c
 *
 *    Description:  This function causes the program to exit
 *					abonrmally.
 *
 *        Created:  02/25/2012 09:52:25 AM
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

int main(int argc, char *argv[]) {
	printf("start\n");
	__builtin_trap(); //no paramets
	printf ("end\n");

	return 0 ;
}

