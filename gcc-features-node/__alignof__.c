/*
 * =====================================================================================
 *
 *       Filename:  __alignof__.c
 *
 *    Description:  和sizeof一样使用，输出是指定类型的数据对齐大小
 *
 *        Created:  04/07/2012 06:19:06 PM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */
#include	<stdio.h>

int main(int argc, char *argv[]) {
	printf("__alignof__(char)=%d\n", __alignof__(char));

	return 0 ;
}

