/*
 * =====================================================================================
 *
 *       Filename:  get-sys-HZ.c
 *       get sys run-time HZ
 *
 *        Created:  07/09/2012 01:03:22 PM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <unistd.h>

int main()
{
    printf ( "sys HZ:%ld\n", sysconf(_SC_CLK_TCK ));
    return 0;
}
