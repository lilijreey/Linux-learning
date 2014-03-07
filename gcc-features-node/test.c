/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Created:  10/27/2012 10:38:48 PM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>


// Test visibility hidden 
void hidden();
void stat() ;
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */

int
main(int argc, char *argv[])
{
    char fmt[]="xx%c"; // fmt not a literal
    //char *fmt="xx%c is ok
    printf(fmt, 's');

    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

