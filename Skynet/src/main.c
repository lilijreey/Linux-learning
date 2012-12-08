#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "log.h"
#include "conf_parser/config.h" // 解析配置文件

#include "bindconf.h"
#include "daemon.h"
#include "dll.h"
#include "net.h"
#include "service.h"
#include "util.h"

static const char version[] = "1.1.1";

char* prog_name;
char* current_dir;

void show_banner()
{
	char feature[256];
	int pos = 0;
	
#ifdef DEBUG	
	pos = sprintf (feature + pos, "-DDEBUG -g ");
#endif
	pos = sprintf (feature + pos, "-DEPOLL_MODE ");

	INFO_LOG("Skynet Server v%s (C) 2011-2013 report bugs to <lilijreey@gmail.com>", version);
	INFO_LOG("Compiled at %s %s, flag: %s\n", __DATE__, __TIME__, pos ? feature : "");
}

// ./Online ./conf/bench.conf id ip port
static inline void
show_usage()
{
	INFO_LOG("Usage: %s bench.conf server-id ip port\n", prog_name);
	exit(-1);
}

static inline void
parse_args(int argc, char** argv)
{
	prog_name    = strdup(argv[0]);
	current_dir  = get_current_dir_name();
	show_banner();
	if ( (argc < 4) || !strcmp(argv[1], "--help") || !strcmp(argv[1], "-h") ) {
		show_usage();
	}
}

int main(int argc, char* argv[])
{
	parse_args(argc, argv);

	if (config_init(argv[1]) == -1) {
		BOOT_LOG(-1, "Failed to Parse File '%s'", argv[1]);
	}

	daemon_start(argc, argv);

    bindconf.bind_num = 1;
    bindconf.configs[0].online_id = atoi(argv[2]);
    bindconf.configs[0].online_name[0] = 0;
    strncpy( bindconf.configs[0].bind_ip, argv[3], 16 );
    bindconf.configs[0].bind_port = atoi(argv[4]);
    bindconf.configs[0].restart_cnt = 0;

	// load bind config file
	//load_bind_file(config_get_strval("bind_conf"));
	// init log files
//	log_init( config_get_strval("log_dir"), config_get_intval("log_level", log_lvl_trace),
//				config_get_intval("log_size", 1<<30), config_get_intval("max_log_files", 100),
//				config_get_strval("log_prefix") );
	//char buffer[128] = { 0 };
	//snprintf(buffer, sizeof(buffer), "%s/%02d", config_get_strval("log_dir"), bindconf.configs[0].online_id);
	log_init_ex( config_get_strval("log_dir"), 
				config_get_intval("log_level", log_lvl_trace),
				config_get_intval("log_size", 1<<30), 
				config_get_intval("max_log_files", 100), 
				config_get_strval("log_prefix") ,
				config_get_intval("log_save_next_file_interval_min", 0) );



	socket_timeout = config_get_intval("cli_socket_timeout", 0);
	page_size      = config_get_intval("incoming_packet_max_size", -1);
	if (page_size <= 0) {
		page_size = def_page_size;
	}

	register_data_plugin(config_get_strval("data_dll_file"));
	register_plugin(config_get_strval("dll_file"), 0);

	net_init(max_fd_num, max_fd_num);
	if (dll.init_service && (dll.init_service(1) != 0)) {
		BOOT_LOG(-1, "FAILED TO INIT PARENT PROCESS");
	}

	bind_config_t* bc = get_bind_conf();
	int   i;
	pid_t pid;
	for ( i = 0; i != bc->bind_num; ++i ) {
		bind_config_elem_t* bc_elem = &(bc->configs[i]);
		shmq_create(bc_elem);

		if ( (pid = fork ()) < 0 ) {
			BOOT_LOG(-1, "fork child process");
		} else if (pid > 0) { //parent process
			close_shmq_pipe(bc, i, 0);
			do_add_conn(bc_elem->sendq.pipe_handles[0], fd_type_pipe, 0, bc_elem);
			net_start(bc_elem->bind_ip, bc_elem->bind_port, bc_elem);
		} else { //child process
			g_listen_port = bc_elem->bind_port;
			run_worker_process(bc, i, i + 1);
		}
	}

	while ((stop == 0) || (dll.fini_service && (dll.fini_service(1) != 0))) {
		net_loop(-1, page_size, 1);
	}

	net_exit();
	unregister_data_plugin();
	unregister_plugin();
	shmq_destroy(0, bc->bind_num);
	daemon_stop();

	return 0;
}
