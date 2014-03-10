/**
 * @file     exe.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     03/07/2014 01:56:28 PM
 *
 */
#include <stdlib.h>

#include <stdio.h>
       #include <unistd.h>

       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

int
main(int argc, char *argv[])
{
    int i;
//    int i = atoi(argv[1]);
    i = open("./exe.c", O_RDONLY);
    printf("yyyyyyyyypip fd write :%d \n", i);
    i = write(4, "hello", 6);
    printf("yyyyyy write :%d\n", i);
    return 0;
}				/* ----------  end of function main  ---------- */
