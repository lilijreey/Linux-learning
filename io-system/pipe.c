/**
 * @file     pipe.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     03/05/2014 10:23:37 AM
 *
 */
#include <stdlib.h>
#include <unistd.h>
#include "../zhao-utility/zhao/tools.h"

//pipe + fork + exec child read and write
//is ok
int
main(int argc, char *argv[])
{
    int p[2];
    char buf[43];
    int i;

    pipe(p);

    i = fork();

    if (i  == -1) {
        perror("fork failed");
        exit(1);
    } else if (i > 0) {
        i = read(p[0], buf, 32);
        printf("read :%d | %s\n",i, buf);
    } else {
        snprintf(buf, 43,"%d", p[1]);
        printf("xx print:%s\n", buf);
        if (-1 == execl("./exe", buf, NULL))
        {
            perror("execl failed");
        }
    }

    return 0;
}				/* ----------  end of function main  ---------- */
