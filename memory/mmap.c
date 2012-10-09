/*
 * =====================================================================================
 *
 *       Filename:  mmap.c
 *
 *    Description:  
 *
 *        Created:  12/30/2011 09:57:14 PM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */
#include	<stdio.h>
#include	<unistd.h>
//#include	<stdlib.h>
//#include	<string.h>
//#include	<zhao/tools.h>

int main() {
	//得到页大小的三种方法
	printf("system page size:%ld\n", sysconf(_SC_PAGESIZE)) ;
	//linux provide
	printf("system page size:%d\n", getpagesize()) ;
//	PAGE_SIZE
	return 0 ;
}

