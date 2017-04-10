/**
 * @file     check_file_exist.c
 *           
 *           access
 *           stat
 *           unlink
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     02/28/2014 07:05:30 PM
 *
 */
#include <stdlib.h>


#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define	NN "./xxxx"			/*  */

#if 0
int
main(int argc, char *argv[])
{

    int n;
    int i;
    int fd;

    fd = open(NN, O_CREAT| O_RDWR, 0777);

    for (i=0; i < 10; ++i) {

        n =access(NN, W_OK);
        if (n == -1)
        {
            perror("access failed");
            if (errno == ENOENT) {
                fd = open(NN, O_CREAT| O_RDWR, 0777);
                printf("no file\n");
            }
        }
        write(fd, "xxx\n", 4);
        printf("is ok\n");
        sleep(1);

    }

    close(fd);


    return 0;
}				/* ----------  end of function main  ---------- */

#endif

#if 0

// open -> access-> unlink -> access-> write
int
main(int argc, char *argv[])
{
//    if (access("./dup2.c", R_OK) < 0 )
//        perror("access");
    int fd;

    fd = open(NN, O_CREAT| O_RDWR, 0777);
    write(fd,"xxx", 3);
    assert(fd > 0);
    printf("fd :%d\n", fd);

    if (access(NN, W_OK) < 0) {
        perror("access 1 failed");
        return 0;
    }

    if (unlink(NN) < 0) {
        perror("unlink failed");
        return 0;
    }

    if (access(NN, R_OK) < 0) {
        perror("access 2 failed");
        return 0;
    }

    return 0;
}				/* ----------  end of function main  ---------- */

#endif 


int
main(int argc, char *argv[])
{
    int fd ;
    fd = open(NN, O_CREAT| O_RDWR, 0777);
    write(fd, "xxx", 3);

    if (rename(NN, "rename") < 0) {
        perror("rename");
        return 0;
    }


    return 0;
}				/* ----------  end of function main  ---------- */

