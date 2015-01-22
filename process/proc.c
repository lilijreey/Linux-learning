/**
 * @file     proc.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     04/21/2014 02:24:45 PM
 *
 */
#include <stdlib.h>
       #include <unistd.h>
#include <stdio.h>



int
main(int argc, char *argv[])
{
    // 都取的是 /utmp 文件
    printf("login name:%s\n", getlogin());
    perror("login bad");
    return 0;
}				/* ----------  end of function main  ---------- */
