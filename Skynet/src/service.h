#ifndef SKYNET_SERVER_SERVICE_H_
#define SKYNET_SERVER_SERVICE_H_

#include <time.h>

#include <glib/ghash.h>

#include "bindconf.h"
#include "daemon.h"

struct shm_queue;
struct shm_block;

typedef struct fdsession {
	int			fd;
	uint32_t	id;
	uint16_t	remote_port;
	uint32_t	remote_ip;
} fdsession_t;

typedef struct fd_array_session {
	int			count;
	GHashTable*	cn;
} fd_array_session_t;

typedef struct {
	int					idle_timeout;
	bind_config_elem_t*	bc_elem;
} config_cache_t;

extern int					is_parent;
extern fd_array_session_t	fds;
extern config_cache_t		config_cache;

void handle_recv_queue();
int  handle_close(int fd);

void run_worker_process(bind_config_t* bc, int bc_elem_idx, int n_inited_bc);
void restart_child_process(bind_config_elem_t* bc_elem);

static inline fdsession_t*
get_fdsess(int fd)
{
	return (fdsession_t*)g_hash_table_lookup(fds.cn, &fd);
}

#endif // SKYNET_SERVER_SERVICE_H_
