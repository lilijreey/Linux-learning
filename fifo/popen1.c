/*
 * =====================================================================================
 *
 *       Filename:  popen1.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  04/05/2011 01:04:35 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#include	<stdio.h>
#include	<string.h>

#include	"myUtility.h"
int main()
{
	FILE *uname_fp = NULL ;
	char buf[1024] ;

	memset(buf, 0, sizeof(buf)) ;
	//open pipe
	EV_TEST(NULL, uname_fp, popen("uname -a", "r"))
	//read in stream from uname_fp
	fgets(buf, sizeof(buf), uname_fp) ;
	fputs(buf, stdout) ;
	
	E_TEST(-1, pclose(uname_fp))

	//write to wc
	EV_TEST(NULL, uname_fp, popen("wc -c", "w"))
	fputs(buf, uname_fp) ;

	E_TEST(-1, pclose(uname_fp))

	return 0 ;
}
