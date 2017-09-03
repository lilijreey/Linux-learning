/**
 * @file     unix_sock_cli.c
 *           
 * @author   lili  <lilijreey@gmail.com>
 * @date     09/03/2017 09:43:11 AM
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "unixSocket.h"
//client 从stdin 读入数据发送给server

void benckmark(int fd) {
    //不停写数据

    char buf[4096];
    size_t n;                                                  
    for (int i=0; i< 10000000; ++i) {
        n += write(fd, buf, 4096);                                  
    }                                                       
    
    printf("client write over %lu(bytes)\n", n);

}

const char path[] = "/tmp/xx.socket";
int main() {

    char buf[256];
    int fd = connect_unix_stream_socket(path);
    printf("connect unix domain socket [%s] successed \n", path);


    benckmark(fd);

    /*int n;                                                  */
    /*while ((n = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {*/
    /*    write(fd, buf, n);                                  */
    /*}                                                       */

    /*if (n == -1) {*/
    /*    close(fd);*/
    /*}             */

    return 0;
}
