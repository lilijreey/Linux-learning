/*
 * =====================================================================================
 *
 *       Filename:  memroy.c
 *
 *    Description:  allocating lots of memory
 *
 *        Version:  1.0
 *        Created:  03/19/2011 06:20:48 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */


#include	<stdio.h>
#include	<stdlib.h>


#define	A_M		(1024 * 1024)			/*  */
#define ALLOC	1024
#if 0
int main()
{
	char *some_memory = NULL ;
	int alloc_count = 0 ;

	while(alloc_count < (ALLOC *2))
	{
		some_memory = (char *) malloc(A_M) ;
		if(NULL != some_memory) {
			alloc_count++ ;
			sprintf(some_memory, "Hello, memory") ;
			printf("%s -now allocated %d megabytes\n", some_memory, alloc_count) ;
		} else {
			exit(1) ;
		}
	}
	char c ;
	scanf("%c", &c) ;
	return 0 ;
}
#endif

int main()
{
	char *some_memory = NULL ;
	int count = 0 ;

	while(1) {
		for(int i =0 ; i < 1024;  i++) {
			some_memory = (char *) malloc(1024) ;
			if(NULL == some_memory) {
				exit(1) ;
			} else {
				sprintf(some_memory, "Hello memory") ;
			}
		}
		count++ ;
		printf("New allocated %d Megabytes\n", count) ;
	}
	return 0 ;
}
