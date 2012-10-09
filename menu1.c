/*
 * =====================================================================================
 *
 *       Filename:  menu1.c
 *
 *    Description:  p145 ,a select menu item, show your chose
 *
 *        Version:  1.0
 *        Created:  03/17/2011 08:50:55 PM
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
#include	<unistd.h>

/*a menu  */
static char const* menu[] = {
	"a - add new record",
	"d - delete record",
	"q - quit",
	NULL
} ;


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  getchoice
 *  Description:  
 * =====================================================================================
 */
int 	
getchoice ( const char *greet, char const **menu, FILE *in, FILE *out )
{
	int choice = 0 ;
	char const* *option = menu ;


	while(1){
		fprintf(out, "Choice:%s\n", greet) ;
		while(*option){ /* show menu */
			fprintf(out, "%s\n", *option++) ;
		}
		do{
			choice = fgetc(in) ;	
		}while('\n' == choice) ;
		/* test if correct choice */
		option = menu ;
		int correct_choice = 0 ;
		while(*option) {
			if(choice == (*option)[0])
				correct_choice = choice ;
				option++ ;
		}
		if(0 == correct_choice) {
			fprintf(out, "Incorrect choice, select again\n") ;
			option = menu ; //renew show menu
			continue ;}
		else {
			break ;
		}
	} 				/* -----  end while  ----- */
	return choice ;

}		/* -----  end of function getchoice  ----- */
int main()
{
	int choice = 0 ;
	FILE *in = stdin ;
	FILE *out = stdout ;

	if(!isatty(fileno(stdout))) {
		fprintf(stderr, "You are not a terminal, OK\n") ;
	}

	in = fopen("/dev/tty", "r") ;
	out = fopen("/dev/tty", "w") ;
	if(0 == in || 0 == out) {
		perror("fopen") ;
		exit(1) ;
	}
	do {
		choice = getchoice("Please select an action", menu, in, out) ;
		printf("You have chosen: %c\n", choice) ;
	} while ( choice != 'q' );				/* -----  end do-while  ----- */
	
	return EXIT_SUCCESS ;

}
