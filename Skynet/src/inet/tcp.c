#include <assert.h>
#include <errno.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#include "tcp.h"

int set_io_blockability(int fd, int nonblock)
{
	int val;
	if (nonblock) {
		val = (O_NONBLOCK | fcntl(fd, F_GETFL));
	} else {
		val = (~O_NONBLOCK & fcntl(fd, F_GETFL));
	}
	return fcntl(fd, F_SETFL, val);
}

int set_sock_snd_timeo(int sockfd, int millisec)
{
	struct timeval tv;

	tv.tv_sec  = millisec / 1000;
	tv.tv_usec = (millisec % 1000) * 1000;

	return setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));
}

int set_sock_rcv_timeo(int sockfd, int millisec)
{
	struct timeval tv;

	tv.tv_sec  = millisec / 1000;
	tv.tv_usec = (millisec % 1000) * 1000;

	return setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
}

int safe_tcp_recv(int sockfd, void* buf, int bufsize)
{
	int cur_len;

recv_again:
	cur_len = recv(sockfd, buf, bufsize, 0);
	//connection closed by client
	if (cur_len == 0) {
		return 0;
	} else if (cur_len == -1) {
		if (errno == EINTR) {
			goto recv_again;
		}
	}

	return cur_len;
}

int safe_tcp_recv_n(int sockfd, void* buf, int total)
{
	assert(total > 0);

	int recv_bytes, cur_len;

	for (recv_bytes = 0; recv_bytes < total; recv_bytes += cur_len)	{
		cur_len = recv(sockfd, buf + recv_bytes, total - recv_bytes, 0);
		// connection closed by client
		if (cur_len == 0) {
			return 0;
		} else if (cur_len == -1) {
			if (errno == EINTR) {
				cur_len = 0;
			} else if (errno == EAGAIN)	{
				return recv_bytes;
			} else {
				return -1;
			}
		}
	}

	return recv_bytes;
}

int safe_tcp_send_n(int sockfd, const void* buf, int total)
{
	assert(total > 0);

	int send_bytes, cur_len;

	for (send_bytes = 0; send_bytes < total; send_bytes += cur_len) {
		cur_len = send(sockfd, buf + send_bytes, total - send_bytes, 0);
		if (cur_len == -1) {
			if (errno == EINTR) {
				cur_len = 0;
			} else if (errno == EAGAIN) {
				return send_bytes;
			} else {
				return -1;
			}
		}
	}

	return send_bytes;
}

int safe_socket_listen(const char* ipaddr, in_port_t port, int type)
{
	int err;
	int listenfd;
	int reuse_addr = 1;

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family  = AF_INET;
	servaddr.sin_port    = htons(port);
	if (ipaddr) {
		inet_pton(AF_INET, ipaddr, &servaddr.sin_addr);
	} else {	
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}

	if ((listenfd = socket(AF_INET, type, 0)) == -1) {
		return -1;
	}

	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof reuse_addr);

	if (bind(listenfd, (void*)&servaddr, sizeof(servaddr)) == -1) {
		err   = errno;
		close(listenfd);
		errno = err;
		return -1;
	}

	if (type == SOCK_STREAM && listen(listenfd, 1024) == -1) {
		err   = errno;
		close(listenfd);
		errno = err;
		return -1;
	}

	return listenfd;
}

int safe_tcp_accept(int sockfd, struct sockaddr_in* peer, int nonblock)
{
	int err;
	int newfd;

	for ( ; ; ) {
		socklen_t peer_size = sizeof(*peer);
		newfd = accept(sockfd, (void*)peer, &peer_size);
		if (newfd >= 0) {
			break;
		} else if (errno != EINTR) {
			return -1;
		}
	}

	if (nonblock && (set_io_blockability(newfd, 1) == -1)) {
		err   = errno;
		close(newfd);
		errno = err;
		return -1;
	}

	return newfd;
}

int safe_tcp_connect(const char* ipaddr, in_port_t port, int timeout, int nonblock)
{
	int err;
	struct sockaddr_in peer;

	memset(&peer, 0, sizeof(peer));
	peer.sin_family  = AF_INET;
	peer.sin_port    = htons(port);
	if (inet_pton(AF_INET, ipaddr, &peer.sin_addr) <= 0) {
		return -1;
	}

	int sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if ( sockfd == -1 ) {
		return -1;
	}

	if (timeout > 0) {
		set_io_blockability(sockfd, 1);
	}

	struct epoll_event ev;
	int epoll_fd = epoll_create(1);
	if (epoll_fd == -1) {
		goto ret;
	}

	int state = 0;
again:
	if (connect(sockfd, (void*)&peer, sizeof(peer)) == 0) {
		if (nonblock) {
			set_io_blockability(sockfd, 1);
		} else {
			set_io_blockability(sockfd, 0);
		}
		close(epoll_fd);
		return sockfd;
	}

	if ( (errno != EINPROGRESS) || (state != 0) || (timeout <= 0)) {
		goto ret;
	}

	ev.events  = EPOLLOUT;
	ev.data.fd = sockfd;
	for ( ; ; ) {
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
			if (errno == EINTR) {
				continue;
			}
			goto ret;
		}
		break;
	}

	if (epoll_wait(epoll_fd, &ev, 1, timeout * 1000) > 0) {
		++state;
		goto again;
	}

ret:
	err   = errno;
	close(epoll_fd);
	close(sockfd);
	errno = err;
	return -1;
}
