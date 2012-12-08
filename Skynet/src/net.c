#include <assert.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <linux/types.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/mman.h>

#include "log.h"
#include "time/timer.h"
#include "conf_parser/config.h"
#include "inet/tcp.h"

#include "bindconf.h"
#include "daemon.h"
#include "shmq.h"
#include "util.h"

#include "dll.h"
#include "mcast.h"
#include "net_if.h"
#include "service.h"

#include "net.h"

enum {
	trash_size		= 4096,
	mcast_pkg_size	= 8192,
	udp_pkg_size	= 8192
};

struct epinfo epi;
time_t socket_timeout;
int page_size;
int g_listen_port;

static int net_recv(int fd, int max, int is_conn);
static int do_open_conn(int fd, int isconn);

inline void free_cb(struct conn_buf *p)
{
	if (p->sendptr) {
		free (p->sendptr);
		p->sendptr = NULL;
	}
	if (p->recvptr) {
		munmap (p->recvptr, page_size);
		p->recvptr = NULL;
	}

	p->recvlen = 0;
	p->sendlen = 0;
} 
	
inline void del_from_close_queue (int fd)
{
	if (epi.fds[fd].flag & CN_NEED_CLOSE) {
		epi.fds[fd].flag &= ~CN_NEED_CLOSE;
		list_del_init (&epi.fds[fd].list);
	}
}
inline void del_from_etin_queue (int fd)
{
	if (epi.fds[fd].flag & CN_NEED_POLLIN) {
		epi.fds[fd].flag &= ~CN_NEED_POLLIN;
		list_del_init (&epi.fds[fd].list);
		TRACE_LOG ("del fd=%d from etin queue", fd);
	}
}
inline void add_to_etin_queue (int fd)
{
	if (!(epi.fds[fd].flag & (CN_NEED_CLOSE | CN_NEED_POLLIN))) {
		list_add_tail (&epi.fds[fd].list, &epi.etin_head);
		epi.fds[fd].flag |= CN_NEED_POLLIN;
		TRACE_LOG ("add fd=%d to etin queue", fd);
	}
}

inline void add_to_close_queue(int fd)
{
	del_from_etin_queue (fd);
	if (!(epi.fds[fd].flag & CN_NEED_CLOSE)) {
		list_add_tail (&epi.fds[fd].list, &epi.close_head);
		epi.fds[fd].flag |= CN_NEED_CLOSE;
		TRACE_LOG("add fd=%d to close queue, %x", fd, epi.fds[fd].flag);
	}
}

inline void iterate_close_queue()
{
	struct list_head *l, *p;
	struct fdinfo *fi;

	list_for_each_safe (p, l, &epi.close_head) {
		fi = list_entry (p, struct fdinfo, list);
		if (fi->cb.sendlen > 0) {
			do_write_conn(fi->sockfd);
		}
		do_del_conn(fi->sockfd, is_parent ? 2 : 0);
	}
}

inline void iterate_etin_queue(int max_len, int is_conn)
{
	struct list_head *l, *p;
	struct fdinfo *fi;

	list_for_each_safe (p, l, &epi.etin_head) {
		fi = list_entry(p, struct fdinfo, list);
		if (unlikely(fi->type == fd_type_listen)) {
			//accept
			while (do_open_conn(fi->sockfd, is_conn) > 0) ;
		} else if (net_recv(fi->sockfd, max_len, is_conn) == -1) {
			do_del_conn(fi->sockfd, is_conn);
		}
	}
}

static inline int add_events (int epfd, int fd, uint32_t flag)
{
	struct epoll_event ev;

	ev.events = flag;
	ev.data.fd = fd;
epoll_add_again:
	if (unlikely (epoll_ctl (epfd, EPOLL_CTL_ADD, fd, &ev) != 0)) {
		if (errno == EINTR)
			goto epoll_add_again;
		ERROR_RETURN (("epoll_ctl add %d error: %m", fd), -1);
	}
	return 0; 
}

int mod_events(int epfd, int fd, uint32_t flag)
{
	struct epoll_event ev;

	ev.events = EPOLLET | flag;
	ev.data.fd = fd;

epoll_mod_again:
	if (unlikely (epoll_ctl (epfd, EPOLL_CTL_MOD, fd, &ev) != 0)) {
		ERROR_LOG ("epoll_ctl mod %d error: %m", fd);
		if (errno == EINTR)
			goto epoll_mod_again;
		return -1;
	}

	return 0;
}

int do_add_conn(int fd, uint8_t type, struct sockaddr_in *peer, bind_config_elem_t* bc_elem)
{
	static uint32_t seq = 0;
	uint32_t flag;

	if ((type == fd_type_pipe) || (type == fd_type_mcast) || (type == fd_type_udp)) {
		flag = EPOLLIN;
	} else {
		flag = EPOLLIN | EPOLLET;
	}

	if (add_events(epi.epfd, fd, flag) == -1)
		return -1;

	memset(&epi.fds[fd], 0x0, sizeof(struct fdinfo));
	epi.fds[fd].sockfd = fd;
	epi.fds[fd].type = type;
	epi.fds[fd].id = ++seq;
	if ( seq == 0 ) {
		epi.fds[fd].id = ++seq;
	}
	if (peer) {
		epi.fds[fd].sk.remote_ip = peer->sin_addr.s_addr;
		epi.fds[fd].sk.remote_port = peer->sin_port;
	}
	epi.fds[fd].bc_elem = bc_elem;
	epi.maxfd = epi.maxfd > fd ? epi.maxfd : fd;
	epi.count ++;

	TRACE_LOG("add fd=%d, type=%d, id=%u", fd, type, epi.fds[fd].id);
	return 0;
}

void do_del_conn(int fd, int is_conn)
{
	if (epi.fds[fd].type == fd_type_unused)
		return ;

	if (is_conn == 0) {
		dll.on_fd_closed(fd);
	} else if (is_conn == 1){
		struct shm_block mb;
		mb.id = epi.fds[fd].id;;
		mb.fd = fd;
		mb.type = CLOSE_BLOCK;
		mb.length = sizeof (mb);
		shmq_push(&(epi.fds[fd].bc_elem->recvq), &mb, NULL);
	}

	del_from_etin_queue(fd);
	del_from_close_queue(fd);

	free_cb (&epi.fds[fd].cb);
	epi.fds[fd].type = fd_type_unused;

	//epoll will auto clear epoll events when fd closed
	close (fd);
	epi.count --;

	if (epi.maxfd == fd) {
		int i;
		for (i = fd - 1; i >= 0; i--)
			if (epi.fds[i].type != fd_type_unused)
				break;
		epi.maxfd = i;
	}
	TRACE_LOG ("close fd=%d", fd);
}

static int do_open_conn(int fd, int isconn)
{
	struct sockaddr_in peer;
	int newfd;

	newfd = safe_tcp_accept(fd, &peer, 1);
	if (newfd != -1) {
		do_add_conn(newfd, fd_type_remote, &peer, epi.fds[fd].bc_elem);
		epi.fds[newfd].sk.last_tm = get_now_tv()->tv_sec;

		if (isconn) {
			struct shm_block mb;

			mb.id = epi.fds[newfd].id;
			mb.fd = newfd;
			mb.type = OPEN_BLOCK;
			mb.length = sizeof (mb) + sizeof (struct skinfo);
			if (shmq_push(&(epi.fds[newfd].bc_elem->recvq), &mb, (const uint8_t *)&epi.fds[newfd].sk) == -1)
				do_del_conn(newfd, 2);
		}
	} else if ((errno == EMFILE) || (errno == ENFILE)) {
		add_to_etin_queue(fd);
	} else if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
		del_from_etin_queue(fd);
	}

	return newfd;
}

int do_write_conn(int fd)
{
	int send_bytes;
	
	send_bytes = safe_tcp_send_n(fd, epi.fds[fd].cb.sendptr, epi.fds[fd].cb.sendlen);
	if (send_bytes == 0) {
		return 0;
	} else if (send_bytes > 0) {
		if (send_bytes < epi.fds[fd].cb.sendlen) {
			memmove(epi.fds[fd].cb.sendptr, epi.fds[fd].cb.sendptr + send_bytes, 
					epi.fds[fd].cb.sendlen - send_bytes);
		}

		epi.fds[fd].cb.sendlen -= send_bytes;
		epi.fds[fd].sk.last_tm = get_now_tv()->tv_sec;
	} else {
		ERROR_LOG("failed to write to fd=%d err=%d %s", fd, errno, strerror(errno));
		return -1;
	}

	return send_bytes;
}

static int do_read_conn(int fd, int max)
{
	int recv_bytes;

	if (!epi.fds[fd].cb.recvptr) {
		epi.fds[fd].cb.rcvprotlen = 0;
		epi.fds[fd].cb.recvlen = 0;
		epi.fds[fd].cb.recvptr = mmap (0, page_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (epi.fds[fd].cb.recvptr == MAP_FAILED) 
			ERROR_RETURN (("mmap failed"), -1);
	}

	if (page_size == epi.fds[fd].cb.recvlen) {
		TRACE_LOG ("recv buffer is full, fd=%d", fd);
		return 0;
	}

	recv_bytes = safe_tcp_recv(fd, epi.fds[fd].cb.recvptr + epi.fds[fd].cb.recvlen, 
								max - epi.fds[fd].cb.recvlen);
	if (recv_bytes > 0) {
		epi.fds[fd].cb.recvlen += recv_bytes;
		epi.fds[fd].sk.last_tm  = get_now_tv()->tv_sec;
	//close
	} else if (recv_bytes == 0) {
		//ERROR_LOG("connection [fd=%d ip=0x%X, port=%u] closed by peer", fd, epi.fds[fd].sk.remote_ip, epi.fds[fd].sk.remote_port);
		DEBUG_LOG("connection [fd=%d ip=0x%X, port=%u] closed by peer", fd, epi.fds[fd].sk.remote_ip, epi.fds[fd].sk.remote_port);
		return -1;
	} else { //EAGAIN ...
		ERROR_LOG("recv error: fd=%d errmsg=%s", fd, strerror(errno));
	}

	if (epi.fds[fd].cb.recvlen == max) {
		add_to_etin_queue(fd);
	} else {
		del_from_etin_queue(fd);
	}

	return recv_bytes;
}

static int net_recv(int fd, int max, int is_conn)
{
	int cnt = 0;

	assert (max <= page_size);
	if (epi.fds[fd].type == fd_type_pipe) {
		read (fd, epi.fds[fd].cb.recvptr, max);
		return 0;
	}
	if (do_read_conn(fd, max) == -1) {
		return -1;
	}

	uint8_t* saved_recvptr = epi.fds[fd].cb.recvptr;
parse_again:	
	//unknow protocol length
	if (epi.fds[fd].cb.rcvprotlen == 0) {
		//parse
		epi.fds[fd].cb.rcvprotlen = dll.get_pkg_len(fd, (void*)epi.fds[fd].cb.recvptr, epi.fds[fd].cb.recvlen, is_conn);
		TRACE_LOG("handle_parse pid=%d return %d, buffer len=%d, fd=%d", getpid(),
					epi.fds[fd].cb.rcvprotlen, epi.fds[fd].cb.recvlen, fd);
	}

	//invalid protocol length
	if (unlikely(epi.fds[fd].cb.rcvprotlen > max)) {
		epi.fds[fd].cb.recvptr = saved_recvptr;
		return -1;
	//unknow protocol length
	} else if (unlikely(epi.fds[fd].cb.rcvprotlen == 0)) {
		if (epi.fds[fd].cb.recvlen == max) {
			epi.fds[fd].cb.recvptr = saved_recvptr;
			ERROR_RETURN(("unsupported big protocol, recvlen=%d", epi.fds[fd].cb.recvlen), -1);
		}
	//integrity protocol	
	} else if (epi.fds[fd].cb.recvlen >= epi.fds[fd].cb.rcvprotlen) {
		if (!is_conn) {
			dll.proc_pkg_from_serv(fd, epi.fds[fd].cb.recvptr, epi.fds[fd].cb.rcvprotlen);
		} else {
			struct shm_block mb;
			epi2shm(fd, &mb);
			if (shmq_push(&epi.fds[fd].bc_elem->recvq, &mb, epi.fds[fd].cb.recvptr)) {
				epi.fds[fd].cb.recvptr = saved_recvptr;
				return -1;
			}
		}

		cnt++;
		if (epi.fds[fd].cb.recvlen > epi.fds[fd].cb.rcvprotlen) {
			epi.fds[fd].cb.recvptr += epi.fds[fd].cb.rcvprotlen;
		}
		epi.fds[fd].cb.recvlen    -= epi.fds[fd].cb.rcvprotlen;
		epi.fds[fd].cb.rcvprotlen  = 0;
		if (epi.fds[fd].cb.recvlen > 0) 
			goto parse_again;
	}

	if (epi.fds[fd].cb.recvptr != saved_recvptr) {
		if (epi.fds[fd].cb.recvlen) {
			memmove(saved_recvptr, epi.fds[fd].cb.recvptr, epi.fds[fd].cb.recvlen);
		}
		epi.fds[fd].cb.recvptr = saved_recvptr;
	}

	return cnt;
}

int net_start(const char* listen_ip, in_port_t listen_port, bind_config_elem_t* bc_elem)
{
	int ret_code = -1, listenfd;
	const int bufsize = 240 * 1024;
	g_listen_port = listen_port;

	listenfd = safe_socket_listen(listen_ip, listen_port, SOCK_STREAM);
	if (listenfd != -1) {
		//set nonblock
		set_io_blockability(listenfd, 1);

		setsockopt(listenfd, SOL_SOCKET, SO_RCVBUF, (char *) &bufsize, sizeof (int));
		setsockopt(listenfd, SOL_SOCKET, SO_SNDBUF, (char *) &bufsize, sizeof (int));

		do_add_conn(listenfd, fd_type_listen, 0, bc_elem);
		ret_code = 0;
	}

	BOOT_LOG(ret_code, "Listen on %s:%u", listen_ip, listen_port);
}

static int schedule_output(struct shm_block *mb)
{
	int data_len;
	int fd = mb->fd;

	if (unlikely((fd > epi.maxfd) || (fd < 0))) {
		DEBUG_LOG("discard the message: mb->type=%d, fd=%d, maxfd=%d, id=%u", 
					mb->type, fd, epi.maxfd, mb->id);
		return -1;
	}

	if (epi.fds[fd].type != fd_type_remote || mb->id != epi.fds[fd].id) { 
		TRACE_LOG ("connection %d closed, discard %u, %u block", fd, mb->id, epi.fds[fd].id);
		return -1;
	}

	if (mb->type == FIN_BLOCK && epi.fds[fd].type != fd_type_listen) {
		add_to_close_queue (fd);
		return 0;
	}

	//shm block send
	data_len = mb->length - sizeof (shm_block_t);
	return net_send(fd, mb->data, data_len);
}

int net_init(int size, int maxevents)
{
    //TODO use epoll_create1 instead epoll_create
	if ((epi.epfd = epoll_create(maxevents)) < 0) {
		ERROR_LOG ("epoll_create failed, %s", strerror (errno));
		return -1;
	}

	epi.evs = calloc(maxevents, sizeof(struct epoll_event));
	if (!epi.evs) 
		goto events_fail;

	epi.fds = (struct fdinfo*) calloc (size, sizeof (struct fdinfo));
	if (!epi.fds)
		goto fd_fail;

	epi.fdsize = size;
	epi.maxfd = 0;
	epi.count = 0;
	INIT_LIST_HEAD (&epi.etin_head);
	INIT_LIST_HEAD (&epi.close_head);
	return 0;

fd_fail:	
	free (epi.evs);
events_fail:
	close (epi.epfd);
	ERROR_RETURN (("malloc failed, size=%d", size), -1);
}

void net_exit ()
{
	int i;
	for (i = 0; i < epi.maxfd + 1; i++) {
		if (epi.fds[i].type == fd_type_unused)
			continue;

		free_cb (&epi.fds[i].cb);
		close (i);
	}

	free (epi.fds);
	free (epi.evs);
	close (epi.epfd);
}

static inline void
handle_send_queue()
{
	struct shm_block *mb;
	struct shm_queue *q;

	int i = 0;
	for ( ; i != bindconf.bind_num; ++i ) {
		q = &(bindconf.configs[i].sendq);
		while ( shmq_pop(q, &mb) == 0 ) {
			schedule_output(mb);	
		}
	}
}

int net_loop(int timeout, int max_len, int is_conn)
{
	int pos, nr;
	char trash[trash_size];

	iterate_close_queue();
	iterate_etin_queue(max_len, is_conn);

	nr = epoll_wait(epi.epfd, epi.evs, epi.maxfd + 1, timeout);
	if (unlikely(nr < 0 && errno != EINTR))
		ERROR_RETURN(("epoll_wait failed, maxfd=%d, epfd=%d: %m", epi.maxfd, epi.epfd), -1);

	renew_now();

	if (is_conn) {
		handle_send_queue();
	}

	for (pos = 0; pos < nr; pos++) {
		int fd = epi.evs[pos].data.fd;
		
		if (fd > epi.maxfd || epi.fds[fd].sockfd != fd || epi.fds[fd].type == fd_type_unused) {
			ERROR_LOG("delayed epoll events: event fd=%d, cache fd=%d, maxfd=%d, type=%d", 
						fd, epi.fds[fd].sockfd, epi.maxfd, epi.fds[fd].type);
			continue;
		}

		if ( unlikely(epi.fds[fd].type == fd_type_pipe) ) {
			if (epi.evs[pos].events & EPOLLHUP) {
				if (is_conn) { // Child Crashed
					int pfd = epi.evs[pos].data.fd;
					bind_config_elem_t* bc = epi.fds[pfd].bc_elem;

					CRIT_LOG("CHILD PROCESS CRASHED!\t[olid=%u olname=%s]", bc->online_id, bc->online_name);					

					// close all connections that are related to the crashed child process
					int i;
					for (i = 0; i <= epi.maxfd; ++i) {
						if ((epi.fds[i].bc_elem == bc) && (epi.fds[i].type != fd_type_listen)) {
							do_del_conn(i, is_conn);
						}
					}
					// prevent child process from being restarted again and again forever
					if (bc->restart_cnt++ < 5) {
						restart_child_process(bc);
					}
				} else { // Parent Crashed
					stop = 1;
					return -1;
				}
			} else {
				while ( read(fd, trash, trash_size) == trash_size ) ;
			}
			continue;
		}

		if (epi.evs[pos].events & EPOLLIN) {
			switch (epi.fds[fd].type) {
			case fd_type_listen:
				//accept
				while (do_open_conn(fd, is_conn) > 0) ;
				break;
// 			case fd_type_mcast:
// 			{
// 				static char buf[mcast_pkg_size];
// 				int  i;
// 				for (i = 0; i != 100; ++i) {
// 					int len = recv(fd, buf, mcast_pkg_size, MSG_DONTWAIT);
// 					if (len > 0) {
// 						if (dll.proc_mcast_pkg) {
// 							dll.proc_mcast_pkg((void*)buf, len);
// 						}
// 					} else {
// 						break;
// 					}
// 				}
// 				break;
// 			}
// 			case fd_type_addr_mcast:
// 			{
// 				static char buf[mcast_pkg_size];
// 				int  i;
// 				for (i = 0; i != 100; ++i) {
// 					int len = recv(fd, buf, mcast_pkg_size, MSG_DONTWAIT);
// 					if (len > 0) {
// 						asyncserv_proc_mcast_pkg(buf, len);
// 					} else {
// 						break;
// 					}
// 				}
// 				break;
// 			}
			case fd_type_udp:
			{
				static char buf[udp_pkg_size];
				int  i;
				for (i = 0; i != 100; ++i) {
					int len = recv(fd, buf, udp_pkg_size, MSG_DONTWAIT);
					if (len > 0) {
						dll.proc_udp_pkg(fd,buf,len );
					} else {
						break;
					}
				}
				break;
			}
	
			default:
				if (net_recv(fd, max_len, is_conn) == -1) {
					do_del_conn(fd, is_conn);
				}
				break;
			}
		}

		if (epi.evs[pos].events & EPOLLOUT) {
			if (epi.fds[fd].cb.sendlen > 0 && do_write_conn(fd) == -1) {
				do_del_conn(fd, is_conn);
			}
			if (epi.fds[fd].cb.sendlen == 0) {
				mod_events(epi.epfd, fd, EPOLLIN);
			}
		}

		if (epi.evs[pos].events & EPOLLHUP) {
			do_del_conn(fd, is_conn);
		}
	}

	if (is_conn && socket_timeout) {
		int i;
		for (i = 0; i <= epi.maxfd; ++i) {
			if ((epi.fds[i].type == fd_type_remote)
					&& ((get_now_tv()->tv_sec - epi.fds[i].sk.last_tm) >= socket_timeout)) {
				do_del_conn(i, is_conn);
			}
		}
	}

	if(!is_conn) {
		time_t now_sec = get_now_tv()->tv_sec;
		if (dll.proc_events) {
			dll.proc_events(now_sec);
		}
		handle_recv_queue();

		// syn addr
		if (now_sec > next_syn_addr_tm) {
			send_addr_mcast_pkg(addr_mcast_syn_pkg);
		}
		// del expired addrs
		if (now_sec > next_del_addrs_tm) {
			del_expired_addrs();
		}
	}

	return 0;
}

//使得work进程可以支持udp.
//bind udp 端口,然后在proc_udp_pkg 中处理得到的数据
int bind_udp_socket( const char* ipaddr, in_port_t port )
{
	int err;
	int listenfd;
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family  = AF_INET;
	servaddr.sin_port    = htons(port);
	if (ipaddr) {
		inet_pton(AF_INET, ipaddr, &servaddr.sin_addr);
	} else {	
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}

	if ((listenfd = socket(AF_INET,SOCK_DGRAM , 0)) == -1) {
		return -1;
	}

	if (bind(listenfd, &servaddr, sizeof(servaddr)) == -1) {
		err   = errno;
		close(listenfd);
		errno = err;
		return -1;
	}
	return do_add_conn(listenfd, fd_type_udp, &servaddr, 0);
}

