#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>

#include <glib.h>

#include "log.h"
#include "time/timer.h"
#include "conf_parser/config.h"

#include "dll.h"
#include "mcast.h"
#include "net_if.h"
#include "util.h"
#include "shmq.h"

#include "service.h"

int					is_parent = 1;
fd_array_session_t	fds;
config_cache_t		config_cache;

static inline void
add_fdsess(fdsession_t* fdsess)
{
	g_hash_table_insert(fds.cn, &(fdsess->fd), fdsess);
	++(fds.count);
}

static inline void
remove_fdsess(int fd)
{
	g_hash_table_remove(fds.cn, &fd);
	--(fds.count);
}

static inline void
free_fdsess(void* fdsess)
{
	g_slice_free1(sizeof(fdsession_t), fdsess);
}

static inline int
handle_init(bind_config_elem_t* bc_elem)
{
	config_cache.idle_timeout = config_get_intval("idle_timeout", 10);
	config_cache.bc_elem      = bc_elem;

	fds.cn = g_hash_table_new_full(g_int_hash, g_int_equal, 0, free_fdsess);

	// create multicast socket if function `proc_mcast_pkg` is defined
	if (dll.proc_mcast_pkg && (create_mcast_socket() == -1)) {
		// return -1 if fail to create mcast socket
		return -1;
	}

	if (config_get_strval("addr_mcast_ip")) {
		if (create_addr_mcast_socket() == 0) {
			send_addr_mcast_pkg(addr_mcast_1st_pkg);
		} else {
			// return -1 if fail to create mcast socket
			return -1;
		}
	}

	return (dll.init_service ? dll.init_service(0) : 0);	
}

static inline int
handle_fini()
{
	int i;
	for (i = 0; i <= epi.maxfd; ++i) {
		if ( (epi.fds[i].type == fd_type_remote) && (epi.fds[i].cb.sendlen > 0) ) {
			return 0;
		}
	}

	if ( dll.fini_service && (dll.fini_service(0) != 0) ) {
		return 0;
	}

	g_hash_table_destroy(fds.cn);
	return 1;
}

static inline int
handle_open(const shm_block_t* mb)
{
	fdsession_t* fdsess = get_fdsess(mb->fd);
	if (fdsess || (mb->length != (sizeof(shm_block_t) + sizeof(skinfo_t)))) {
		ERROR_LOG("handle_open OPEN_BLOCK, fd=%d length=%d", mb->fd, mb->length);
		return -1;
	} else {
		fdsess               = g_slice_alloc(sizeof *fdsess);
		fdsess->fd           = mb->fd;
		fdsess->id           = mb->id;
		fdsess->remote_port  = *(uint16_t*)mb->data;
		fdsess->remote_ip    = *(uint32_t*)&mb->data[2];
		add_fdsess(fdsess);
		dll.on_client_conn_opened(fdsess);
	}

	return 0;
}

int handle_close(int fd)
{
	fdsession_t* fdsess = get_fdsess(fd);
	if (!fdsess) {
		ERROR_RETURN( ("connection %d had already been closed", fd), -1 );
	}

	assert(fds.count > 0);

	dll.on_client_conn_closed(fd);

	remove_fdsess(fd);
	return 0;
}

static inline void
handle_process(uint8_t* recvbuf, int rcvlen, int fd)
{
	fdsession_t* fdsess = get_fdsess(fd);
	if (fdsess) 
		dll.proc_pkg_from_client(recvbuf, rcvlen, fdsess);
}

void handle_recv_queue()
{
	struct shm_queue* recvq = &(config_cache.bc_elem->recvq);
	struct shm_queue* sendq = &(config_cache.bc_elem->sendq);

	struct shm_block* mb;
	while (shmq_pop(recvq, &mb) == 0) {
		switch (mb->type) {
		case DATA_BLOCK:
			if ( mb->length > sizeof(*mb) ) {
				handle_process(mb->data, mb->length - sizeof(*mb), mb->fd);
			}
			break;
		case OPEN_BLOCK:
			if ( handle_open(mb) == -1 ) {
				mb->type    = FIN_BLOCK;
				mb->length  = sizeof(*mb);
				shmq_push(sendq, mb, NULL);
			}
			break;
		case CLOSE_BLOCK:
			handle_close(mb->fd);
			break;		
		default:
			break;
		}
	}
}

/**
 * run_worker_process
 *   @bc - bind config
 *   @bc_elem_idx - index bc->configs
 *   @n_inited_bc - number of inited bc->configs
 *
 */
void run_worker_process(bind_config_t* bc, int bc_elem_idx, int n_inited_bc)
{
	bind_config_elem_t* bc_elem = &(bc->configs[bc_elem_idx]);

	char prefix[10] = { 0 };
	int  len       = snprintf(prefix, 8, "%u", bc_elem->online_id);
	prefix[len] = '_';
	log_init_ex( config_get_strval("log_dir"), 
				config_get_intval("log_level", log_lvl_trace),
				config_get_intval("log_size", 1<<30), 
				config_get_intval("max_log_files", 100), 
				prefix ,
				config_get_intval("log_save_next_file_interval_min", 0) );
	is_parent = 0;

	// release resources inherited from parent process
	close_shmq_pipe(bc, n_inited_bc, 1);
	shmq_destroy(bc_elem, n_inited_bc);
	net_exit();

	daemon_set_title("%s-%u", prog_name, bc_elem->online_id);	

	net_init(max_fd_num, 100);
	do_add_conn(bc_elem->recvq.pipe_handles[0], fd_type_pipe, 0, 0);

	if ( handle_init(bc_elem) != 0 ) {
		ERROR_LOG("fail to init worker process. olid=%u olname=%s", bc_elem->online_id, bc_elem->online_name);
		goto fail;
	}
	
	while ( !stop || !handle_fini() ) {
		net_loop(100, page_size, 0);
	}

fail:
	do_destroy_shmq(bc_elem);
	net_exit();
	unregister_data_plugin();
	unregister_plugin();
	free_argv();
	free(prog_name);
	free(current_dir);

	exit(0);
}

void restart_child_process(bind_config_elem_t* bc_elem)
{
	close(bc_elem->recvq.pipe_handles[1]);
	do_del_conn(bc_elem->sendq.pipe_handles[0], 2);
	do_destroy_shmq(bc_elem);

	shmq_create(bc_elem);

	bind_config_t* bc = get_bind_conf();
	int i = get_bind_conf_idx(bc_elem);
	pid_t pid;

	if ( (pid = fork ()) < 0 ) {
		CRIT_LOG("fork failed: %s", strerror(errno));
	} else if (pid > 0) { //parent process
		close_shmq_pipe(bc, i, 0);
		do_add_conn(bc_elem->sendq.pipe_handles[0], fd_type_pipe, 0, bc_elem);
	} else { //child process
		run_worker_process(bc, i, bc->bind_num);
	}
}

