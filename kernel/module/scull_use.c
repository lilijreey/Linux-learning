/**
 * @file     scull_use.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     2018年02月24日 16时26分20秒
 *
 */
#include <stdio.h>

#include <unistd.h>
#include <stdlib.h>
       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

#include "lock.h"
int main() {
    int fd = open("/dev/scull0", O_RDWR);
    if (fd < 0) {
        perror("open failed");
        exit(0);
    }

    printf("open ok %d\n", fd);
    int ret = read(fd, 0, 0/* ignore */);
    if (ret < 0) {
        perror("read failed");
        exit(0);
    }

    printf("lock 0 state:%u\n", ret);
    return 0;


}

