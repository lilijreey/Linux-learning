/**
 *============================================================
 *  @file        tcp.h
 *  @brief      Essential TCP socket interface
 * 
 *  platform   Linux
 *
 *============================================================
 */

#ifndef TCP_H_
#define TCP_H_

#include <netinet/in.h>

/**
  * @brief Set the given fd to be blocking or noblocking
  *
  * @param fd file descriptor to be set.
  * @param nonblock 0 to set the given fd blocking, 1 to set the fd nonblocking.
  *
  * @return 0 on success, -1 on error.
  */
int set_io_blockability(int fd, int nonblock);

/**
  * @brief Set a timeout on sending data. If you want to disable timeout, just simply
  *           call this function again with millisec set to be 0.
  *
  * @param sockfd socket descriptor to be set.
  * @param millisec timeout in milliseconds.
  *
  * @return 0 on success, -1 on error and errno is set appropriately.
  */
int set_sock_snd_timeo(int sockfd, int millisec);

/**
  * @brief Set a timeout on receiving data. If you want to disable timeout, just simply
  *           call this function again with millisec set to be 0.
  *
  * @param sockfd socket descriptor to be set.
  * @param millisec timeout in milliseconds.
  *
  * @return 0 on success, -1 on error and errno is set appropriately.
  */
int set_sock_rcv_timeo(int sockfd, int millisec);

/**
  * @brief Accept a TCP connection
  *
  * @param int sockfd,  the listening fd.
  * @param struct sockaddr_in* peer,  used to return the protocol address of the connected peer process.  
  * @param int nonblock,  0 and the accepted fd will be set blocking, 1 and the fd will be set nonblocking.
  *
  * @return int, the accpected fd on success, -1 on error.
  */
int safe_tcp_accept(int sockfd, struct sockaddr_in* peer, int nonblock);

/**
  * @brief Create a TCP connection
  *
  * @param const char* ipaddr,  the ip address to connect to.
  * @param in_port_t port,  the port to connect to.
  * @param int timeout,  abort the connecting attempt after timeout secs. If timeout is less than or equal to 0, 
  *                                then the connecting attempt will not be interrupted until error occurs.
  * @param int nonblock,  0 and the connected fd will be set blocking, 1 and the fd will be set nonblocking.
  *
  * @return int, the connected fd on success, -1 on error.
  */
int safe_tcp_connect(const char* ipaddr, in_port_t port, int timeout, int nonblock);

/**
  * @brief Create a listening socket fd
  *
  * @param const char* ipaddr,  the binding ip address. If this argument is assigned with 0,
  *                                           then INADDR_ANY will be used as the binding address.
  * @param in_port_t port,  the binding port.
  * @param int type,  type of socket (SOCK_STREAM, SOCK_DGRAM, SOCK_SEQPACKET, SOCK_RAW).
  *
  * @return int, the newly created listening fd on success, -1 on error.
  */
int safe_socket_listen(const char* ipaddr, in_port_t port, int type);

/**
  * @brief Send `total` bytes of data to `sockfd`
  *
  * @param int sockfd,  socket fd to send data to.
  * @param const void* buf,  data to be sent.
  * @param int total,  number of bytes in total to be sent.
  *
  * @return int, number of bytes sent on success, -1 on error.
  */
int safe_tcp_send_n(int sockfd, const void* buf, int total);

/**
  * @brief Receive data from `sockfd`
  *
  * @param int sockfd,  socket fd to receive data from.
  * @param const void* buf,  buffer to hold the receiving data.
  * @param int bufsize,  size of `buf`.
  *
  * @return int, number of bytes receive on success, -1 on error, 0 on connection closed by peer.
  */
int safe_tcp_recv(int sockfd, void* buf, int bufsize);

/**
  * @brief Receive `total` bytes of data from `sockfd`
  *
  * @param int sockfd,  socket fd to receive data from.
  * @param const void* buf,  buffer to hold the receiving data. Size of `buf` should be no less than `total`
  * @param int total,  number of bytes to receive.
  *
  * @return int, number of bytes receive on success, -1 on error, 0 on connection closed by peer.
  */
int safe_tcp_recv_n(int sockfd, void* buf, int total);

#endif // ant_TCP_H_
