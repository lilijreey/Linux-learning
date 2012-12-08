#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include <openssl/md5.h>

int connect_to_svr(const char* ipaddr, uint16_t port)
{
	struct sockaddr_in peer;
	int fd;

	bzero(&peer, sizeof (peer));
	peer.sin_family  = AF_INET;
	peer.sin_port    = htons(port);
	if (inet_pton (AF_INET, ipaddr, &peer.sin_addr) <= 0) {
        return -1;
    }
    
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    connect(sockfd, (const struct sockaddr *)&peer, sizeof(peer));

	return sockfd;
}

int main(int argc, char **argv)
{
    int buf[4096];
    int sockfd = connect_to_svr("10.1.1.5", 8765);
    write(sockfd, "123122", 7);
    ssize_t n = read(sockfd, buf, 4096);
	write(STDIN_FILENO, buf, n);
	putchar('\n');
	if (n == 7) {
    	read(sockfd, buf, 4096);
    	printf("%s\n", buf);
	}
    read(sockfd, buf, 4096);
    printf("%s\n", buf);
}
