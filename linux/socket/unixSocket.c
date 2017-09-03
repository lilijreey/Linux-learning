/**
 * @file     unixSocket.c
 * UNIX 套接字           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     09/03/2017 09:04:36 AM
 *
 */
#include <stdio.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <assert.h>
#include <sys/un.h>
#include <stdlib.h>

int create_unix_stream_socket_listener(const char * sock_path) {
    /*
    * AF_UNIX or AF_LOCAL 是unix socket 的类型
    * UNIX domain socket 同时支持流式(SOCK_STREAM)和报文式(SOCK_DGRAM)两种socket类型
    * 地址格式为一个 文件路径 sockaddr_un
    */
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    assert(fd != -1 && "socket() failed");


#if 0
    include in <sys/un.h>
    struct sockaddr_un {
        sa_family_t sun_family;/* Always AF_UNIX */
        char sun_path[108]; /* Null-terminated socket pathname */
    };
#endif
    struct sockaddr_un addr; //需要填充类型
    bzero(&addr, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, sock_path, sizeof(addr.sun_path)-1);

    //bind 会生产socket 文件，路径必须有读写权限，
    //不能绑定一个已有的socket文件,及时这个socket文件是上次创建的
    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind failed");
        exit(1);
    }
    //一般bind前会调用remove 删除需要绑定的socket文件，因为有可能存在,导致无法绑定

#if 0
    linux 抽象名字
    bind 还可以使用一个不是真实存在的文件，这个是linux提供的特性，饺子Abstract namespace
    使用netstat -na 中显示的以@开头的文件都是抽象名字。 好处是在close bind的fd后会自动删除文件
    要使用这个特性，那么 sun_path的第一个字节必须设置未'\0',后面是一个文件名
    e.g. sum_path=[0] + "xx.socket"
    在bind调用中，第三个参数是 sizeof(sun_family) + 1(第一个0字节)+xx.socket的长度,不算最后的NUL字符
#endif
    
   
    /*
     * 对于流式类型还是需要listen, socket 文件就相当与listen 的port
     */
    if (listen(fd, 5) == -1) {
        perror("bind failed");
        exit(1);
    }

    return fd;
}


int connect_unix_stream_socket(const char *sock_path) {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    assert(fd != -1 && "socket() failed");


    struct sockaddr_un addr; 
    bzero(&addr, sizeof(addr));

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, sock_path, sizeof(addr.sun_path)-1); //需要链接的sockt 文件路径

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect failed");
        exit(1);
    }

    return fd;
}

