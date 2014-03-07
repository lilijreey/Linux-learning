/*
 * =====================================================================================
 *
 *       Filename:  __unbounded.c
 *
 *    Description: 边界检查 __bounded is a gcc builed in attribute 
 *                 this is action to pointer, that is provid 
 *                 memery bound check. but newerly gcc not provided.
 *                 So the __bounded, __unbounded, __ptrvalue is
 *                 define to noting
 *                 e.g #define __bounded // nothing 
 *
 *        Created:  02/25/2012 09:29:40 AM
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

// what is __unbounded action?
int main(int argc, char *argv[]) {
	char str[] = "hello";
	// It equment char * s = str ;
	char * __unbounded s =  str ;
	printf ( "\n" );

	return 0 ;
}

