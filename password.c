/*
 * =====================================================================================
 *
 *       Filename:  password.c
 *
 *    Description:  use termios struct buliting a password 
 *					program, then entry password no echo
 *
 *        Version:  1.0
 *        Created:  03/19/2011 03:45:33 PM
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
#include	<string.h>
#include	<termios.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<fcntl.h>

										/*not has '\0'*/
#define	   PASS_WORD_LENGHT   20		
										/*has '\0' and a overflow char  */
#define	   PASS_WORD_BUFFER_LENGHT   (PASS_WORD_LENGHT +2)		

int main()
{
	int fp ;
	if(-1 == (fp = open("/dev/tty", O_RDWR))) {
		perror("open /dev/tty") ;
		exit(1) ;
	}
	struct termios default_termios, new_termios ; //storage a default termios struct

	//backup default termiso, and copy a new termios form default termios
	if(-1 == tcgetattr(fp, &default_termios)) {
		perror("tcgetattr") ;
		exit(1) ;
	}


	new_termios = default_termios ; 
	new_termios.c_lflag &= ~ECHO ; //close echo mode

	//set termios no echo
	if(-1 == tcsetattr(fp, TCSAFLUSH, &new_termios)) {
		perror("tcsetattr") ;
		exit(1) ;
	}
	char new_passwd[PASS_WORD_BUFFER_LENGHT] ;
	char retype_new_passwd[PASS_WORD_BUFFER_LENGHT] ;

	printf("Entry your new password:") ;
	while(PASS_WORD_LENGHT < strlen(fgets(new_passwd, PASS_WORD_BUFFER_LENGHT, stdin))) {
		printf("\npass word is too long, plase entry again!\n") ;
	}	
	printf("\nRetype new password:") ;

	fgets(retype_new_passwd, PASS_WORD_BUFFER_LENGHT, stdin) ;	
	//check if retype pass word as same as first entry new pass word
	if( 0 == strcmp(new_passwd, retype_new_passwd))
		printf("\nnew pass word is success\n") ;
	else
		printf("\nno match pass word\n") ;

	//resume default termios
	tcsetattr(fp, TCSAFLUSH, &default_termios) ;

	if(-1 == close(fp)) {
		perror("close()") ;
		exit(1) ;
	}

	return 0 ;
}
