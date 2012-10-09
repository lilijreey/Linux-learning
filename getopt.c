/*
 * =====================================================================================
 *
 *       Filename:  getopt.c
 *
 *    Description:  test the getopt func
 *
 *		可用于其他程序处理参数的模板
 *		分为X/Open getopt() -argument
 *		 GNU getopt_long() --argument
 *        Version:  1.0
 *        Created:  03/13/2011 05:20:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Lilijreey (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */

#include	<stdio.h>
#include	<unistd.h>
//show the progream argument usage, and the program infomation
void help_maunal(void)
{
	printf("-c output \"Cao i Ma \"\n") ;  
	printf("-f output \"Fuck you \"\n") ;
	printf("-n=name entry you want to person's name\n") ;
}

#if 0
//use X/Open version getopt not support --
int main(int argc, char * argv[])
{	
	//use your program name substutite unber the name
	const char *program_name = "enter you program name" ; 
	int opt ;
	char no_value ; //no has a value argument 
	char fault_arg ;

	while(-1 != (opt = getopt(argc, argv, ":fn:h")))
		{
			switch (opt) {
				case 'c':	
					printf("Cao ni Ma ") ;
					break;

				case 'f':
				   printf("Fuck you ") ;	
					break;

				case 'n':	
					printf("%s\n", optarg) ;
					break;

				case 'h':
					help_maunal() ;
					break ;

				case '?': //argument error
					fault_arg = (char)optopt ;
					printf("%s: invalid option - '%c'\n", program_name, fault_arg) ;
					printf("Try `%s -h' for more information.\n", program_name) ;
					break ;

				case ':':
					no_value = (char)opt ;
					printf("%s: option '%c' requires an argument\n", program_name, no_value) ;
					printf("Try `%s -h' for more information.\n", program_name) ;
					break ;
				default:	
					break;
			}				/* -----  end switch  ----- */

		}		
	return 0 ;
}

#endif



//GNU version getopt_long it supports --
#define _GNU_SOURCE
#include <getopt.h> //define struct option
int main(int argc, char *argv[])
{
	//use your program name substutite unber the name
	const char *program_name = "enter you program name" ; 
	int opt ;
	char no_value ; //no has a value argument 
	char fault_arg ;
	struct option longopts[] = {
		{"cao", 0, NULL, 'c'},
		{"fuck", 0, NULL, 'f'},
		{"name", 1, NULL, 'n'},
		//any programs to used
		{"help", 0, NULL, 'h'},
		{0, 0, 0, 0} } ;

	while(-1 != (opt = getopt_long(argc, argv, ":cfhn:", longopts, NULL))){

		switch ( opt ) {
			case 'c':	
				printf("Cao ni Ma ") ;
				break;

			case 'f':
			   printf("Fuck you ") ;	
			   break;

			case 'n':	
				printf("%s\n", optarg) ;
				break;

			//any progream to used
			case 'h':
				help_maunal() ;
				break ;

			case '?': //argument error
				fault_arg = (char)optopt ;
				printf("%s: invalid option -- '%c'\n", program_name, fault_arg) ;
				printf("Try `%s -h' or `%s --help' for more information.\n", program_name, program_name) ;
				break ;

			case ':':
				no_value = (char)opt ;
				printf("%s: option '%c' requires an argument\n", program_name, no_value) ;
				printf("Try `%s -h' or `%s --help' for more information.\n", program_name, program_name) ;
				break ;

		}				/* -----  end switch  ----- */
	}	

	return 0 ;
}
