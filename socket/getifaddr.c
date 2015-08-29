/**
 * @file     getifaddr.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     2015年06月12日 12时13分03秒
 *
 */

//#include <sys/socket.h>
//#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
    struct ifaddrs *ifaddr, *ifa;
    int family ;
    char host[256];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }
    /* Walk through linked list, maintaining head pointer so we
       can free list later */

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        family = ifa->ifa_addr->sa_family;

        /* For an AF_INET* interface address, display the address */
        if (family == AF_INET) {
            inet_ntop(family, &((struct sockaddr_in*)ifa->ifa_addr)->sin_addr, host,  256)  ;
            printf("\t%s: address: <%s>\n", ifa->ifa_name, host);
        } else if ( family == AF_INET6) {
//            inet_ntop(family, &((struct sockaddr_in6*)ifa->ifa_addr)->sin6_addr, host,  NI_MAXHOST)  ;
//            printf("\t%s: address: <%s>\n", ifa->ifa_name, host);
        }
    }

    freeifaddrs(ifaddr);
    exit(EXIT_SUCCESS);
}

