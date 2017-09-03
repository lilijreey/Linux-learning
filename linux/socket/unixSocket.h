/**
 * @file     unixSocket.h
 *           
 *
 * @author   lili <lilijreey@gmail.com>
 * @date     09/03/2017 09:39:12 AM
 *
 */

#ifndef  UNIXSOCKET_H
#define  UNIXSOCKET_H


int create_unix_stream_socket_listener(const char * sock_path);
int connect_unix_stream_socket(const char *sock_path);

#endif   /* ----- #ifndef UNIXSOCKET_H ----- */
