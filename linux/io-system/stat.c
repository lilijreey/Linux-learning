/**
 * @file     stat.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     03/03/2014 10:42:51 AM
 *
 */
#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>


int
main(int argc, char *argv[])
{
    struct stat st;
    stat("./stat.c", &st);
    printf("st_dev :%ld\n", st.st_dev);
    printf("st_ino :%ld\n", st.st_ino);
    return 0;
}				/* ----------  end of function main  ---------- */

