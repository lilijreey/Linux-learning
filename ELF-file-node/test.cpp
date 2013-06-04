/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Created:  06/18/2012 09:31:21 PM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */


#include	<stdio.h>
int main() {
    char *str = "HELLO"; //HELLO 保存到.rodata中吗？
    const int SIZ=10;

    printf ( "num is:%d str:%s\n",  SIZ, str );
    return 0;
}
