#include <libant/list.h>
#include <libant/log.h>
#include <libant/timer.h>

#include <async_serv/net_if.h>

#include "test.h"

typedef struct TmrTest {
	list_head_t timer_list;

} tmr_test_t;

tmr_test_t tmr;

int test(void* owner, void* data)
{
	send_pkg_to_client(data, "111v", 5);
	return 0;
}

int init_service(int isparent)
{
	DEBUG_LOG("INIT...");
	if (!isparent) {
		setup_timer();
		INIT_LIST_HEAD(&tmr.timer_list);
	}
	return 0;
}

int fini_service(int isparent)
{
	DEBUG_LOG("FINI...");
	return 0;
}

void proc_events()
{
	handle_timer();
}

int	get_pkg_len(int fd, const void* pkg, int pkglen, int isparent)
{
	if (isparent) {
		int i;

		const char* str = pkg;
		for (i = 0; (i != pkglen) && (str[i] != '\0'); ++i) ;

		if (i != pkglen) {
			net_send(fd, pkg, i + 1);
			return i + 1;
		}
	}

	return 0;
}

int proc_pkg_from_client(void* data, int len, fdsession_t* fdsess)
{
	DEBUG_LOG("%s", data);
	send_pkg_to_client(fdsess, data, len);
	ADD_TIMER_EVENT(&tmr, test, fdsess, get_now_tv()->tv_sec + 5);
	return 0;
}

void proc_pkg_from_serv(int fd, void* data, int len)
{
}

void on_client_conn_closed(int fd)
{
}

void on_fd_closed(int fd)
{
}

