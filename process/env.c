/**
 * @file     env.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     04/19/2014 11:42:40 AM
 *
 */
#include <stdio.h>

extern char **environ;

int
main(int argc, char *argv[])
{
    char * cc = *environ;
    printf("%p\n", cc);
    return 0;
}				/* ----------  end of function main  ---------- */
