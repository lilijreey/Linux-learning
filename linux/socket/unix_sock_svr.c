/**
 * @file     unix_sock_svr.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     09/03/2017 09:38:53 AM
 *
 */
#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>

#include "unixSocket.h"

void benckmark(int fd) {
    //不停读取数据，统计时间, 5G/s 平均,没有丢失
    char buf[4096];
    size_t n;
    size_t toal=0;
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        toal += n;
    }
    printf("total read %lu(bytes) %lu(MB)\n", toal, toal/1024/1024);
}

int main() {

    const char path[] = "/tmp/xx.socket";
    int listenFd = create_unix_stream_socket_listener(path);
    printf("listeing %s\n", path);

    char buf[256];
    for (;;) {
        int clientFd = accept(listenFd, NULL, NULL);
        if (clientFd == -1) {
            perror("accept failed");
            exit(1);
        }
        printf("accept new client %d\n", clientFd);

        benckmark(clientFd);

        /*int n;                                              */
        /*while ((n = read(clientFd, buf, sizeof(buf))) > 0) {*/
        /*    write(STDOUT_FILENO, buf, n);                   */
        /*}                                                   */

        printf("read 0 close peer socket %d\n", clientFd);
        close(clientFd);
    }


    //需要删除socket 文件
    unlink(path);
    close(listenFd);

}

