
#include	<stdio.h>
#include	<stdlib.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
	int
main ( int argc, char *argv[] )
{
	int c , n = 0 ;
	while ((c = getchar()) != 'q') {
		printf("char %3d is %c code %d\n", n++, c, c);
	}
			
	return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
