#ifndef _SERVICE_H_
#define _SERVICE_H_

#include <async_serv/dll.h>

int  init_service();
int  fini_service();
void proc_events();

int	 get_pkg_len(int fd, const void* pkg, int pkglen, int isparent);

int  proc_pkg_from_client(void* data, int len, fdsession_t* fdsess);
void proc_pkg_from_serv(int fd, void* data, int len);

void on_client_conn_closed(int fd);
void on_fd_closed(int fd);
void proc_udp_pkg(int fd,void* data, int len);

#endif
