/**
 *============================================================
 *  @file        mcast.h
 *  @brief      Define the interfaces to send multicast datagram
 * 
 *  compiler   gcc4.1.2
 *  platform   Linux
 *
 *============================================================
 */

#ifndef SKYNET_SERVER_MCAST_H_
#define SKYNET_SERVER_MCAST_H_

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

enum {
	mcast_notify_addr   = 0,
	mcast_reload_text	= 1
};

enum {
	addr_mcast_1st_pkg	= 1,
	addr_mcast_syn_pkg	= 2
};

struct addr_node {
	uint32_t		svr_id;
	char			ip[16];
	unsigned short	port;
	time_t			last_syn_tm;
};

typedef struct addr_node addr_node_t;

extern time_t next_syn_addr_tm;
extern time_t next_del_addrs_tm;

int  create_addr_mcast_socket();
void send_addr_mcast_pkg();
addr_node_t* get_service_ipport(const char* service, unsigned int svr_id);

void del_expired_addrs();
void asyncserv_proc_mcast_pkg(void* data, int len);

//------------------------------------------------------------

int create_mcast_socket();

/**
  * @brief Send out a mcast datagram to the specified `mcast_ip` and `mcast_port`.
  *           Note, length of the data should be no longer than 8192.
  *
  * @param const void* data,  data to send.
  * @param int len,  length of `data`.
  *
  * @return int, number of characters sent on success, -1 on error
  */
int send_mcast_pkg(const void* data, int len);

#ifdef __cplusplus
} // end of extern "C"
#endif

#endif // SKYNET_SERVER_MCAST_H_

