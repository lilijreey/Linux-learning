// C89
#include <stdlib.h>
#include <string.h>
// POSIX
#include <arpa/inet.h>
#include <sys/mman.h>

#include "log.h"
#include "conf_parser/config.h"

// Self-define
#include "bindconf.h"

// global varibles for bindconf.c
bind_config_t bindconf;

enum {
	bind_conf_max_field_num	= 4
};

/**
 * load_bind_file - parse bind config file @file_name
 * 
 * return: 0 on success, otherwise -1
 */
int load_bind_file(uint32_t id, uint16_t port)
{
//	int		ret_code = -1;
//	char*	buf;

    bindconf.bind_num = 1;
    bindconf.configs[0].online_id = id;
    bindconf.configs[0].online_name[0] = 0;
    bindconf.configs[0].bind_ip[0] = 0;
    bindconf.configs[0].bind_port = port;
    bindconf.configs[0].restart_cnt = 0;

    return 0;
// 	if ( mmap_config_file(file_name, &buf) > 0 ) {
// 		char* start = buf;
// 		char* end;
// 		char* field[bind_conf_max_field_num];
// 		bind_config_elem_t* bc;
// 
// 		size_t len = strlen(buf);
// 		while (buf + len > start) {
// 			end = strchr(start, '\n');
// 			if ( end && *end ) {
// 				*end = '\0';
// 			}
// 			if ( (*start != '#') && (str_split(0, start, field, bind_conf_max_field_num) == bind_conf_max_field_num) ) {
// 				bc = &(bindconf.configs[bindconf.bind_num]);
// 				// Online
// 				bc->online_id = atoi(field[0]); // online id
// 				strncpy(bc->online_name, field[1], sizeof(bc->online_name) - 1); // online name
// 				strncpy(bc->bind_ip, field[2], sizeof(bc->bind_ip) - 1); // online ip
// 				bc->bind_port = atoi(field[3]); // online port
// 				// increase bind_num
// 				++(bindconf.bind_num);
// 			}
// 			start = end + 1;
// 
// 			if (bindconf.bind_num > max_listen_fds) {
// 				goto exit_entry;
// 			}
// 		}
// 
// 		munmap(buf, len);
// 		ret_code = 0;
// 	}
// 
// exit_entry:
// 	BOOT_LOG(ret_code, "load bind file:%s", file_name);
}
