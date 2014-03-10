/**
 * @file     socketpair.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     03/10/2014 03:39:37 PM
 *
 */
#include <stdlib.h>

#include <string.h>
#include <sys/types.h>	       /* See NOTES */
       #include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>



int
main(int argc, char *argv[])
{
    int so[2];
    char buf[32]={0};
    if (-1 == socketpair(AF_UNIX, SOCK_STREAM, 0, so)) {
        perror("socketpair");
        exit(1);
    }

    write(so[0], "xxx", 3);
    read(so[1], buf, 3);

    printf("so 0 buf:%s\n", buf);

    write(so[1], "yyy", 3);
    read(so[0], buf, 3);
    printf("so 0 buf:%s\n", buf);

    return 0;
}				/* ----------  end of function main  ---------- */
