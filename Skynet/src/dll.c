#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "log.h"
#include "dll.h"

async_serv_if_t dll;

#define DLFUNC_NO_ERROR(h, v, name) \
    do { \
        v = dlsym(h, name); \
        dlerror(); \
    } while (0)

#define DLFUNC(h, v, name) \
    do { \
        v = dlsym (h, name); \
        if ((error = dlerror ()) != NULL) { \
            ERROR_LOG("dlsym error, %s", error); \
            dlclose(h); \
            h = NULL; \
            goto out; \
        } \
    } while (0)

int register_data_plugin(const char* file_name)
{
    char* error; 
    int   ret_code = 0;
    if (file_name == NULL)
        return 0;
    dll.data_handle = dlopen(file_name, RTLD_NOW | RTLD_GLOBAL);
    if ((error = dlerror()) != NULL) {
        ERROR_LOG("dlopen error, %s", error);
        ret_code = 0;
    }
    BOOT_LOG(ret_code, "dlopen %s", file_name);
}

int register_plugin(const char* file_name, int flag)
{
    char* error; 
    int   ret_code = -1;

    dll.handle = dlopen(file_name, RTLD_NOW);
    if ((error = dlerror()) != NULL) {
        ERROR_LOG("dlopen error, %s", error);
        goto out;
    }

    DLFUNC_NO_ERROR(dll.handle, dll.init_service, "init_service");
    DLFUNC_NO_ERROR(dll.handle, dll.fini_service, "fini_service");
    DLFUNC_NO_ERROR(dll.handle, dll.proc_events, "proc_events");
    DLFUNC_NO_ERROR(dll.handle, dll.proc_mcast_pkg, "proc_mcast_pkg");
    DLFUNC_NO_ERROR(dll.handle, dll.proc_udp_pkg, "proc_udp_pkg");

    DLFUNC(dll.handle, dll.get_pkg_len, "get_pkg_len");
    DLFUNC(dll.handle, dll.on_client_conn_opened, "on_client_conn_opened");
    DLFUNC(dll.handle, dll.proc_pkg_from_client, "proc_pkg_from_client");
    DLFUNC(dll.handle, dll.proc_pkg_from_serv, "proc_pkg_from_serv");
    DLFUNC(dll.handle, dll.on_client_conn_closed, "on_client_conn_closed");
    DLFUNC(dll.handle, dll.on_fd_closed, "on_fd_closed");

    DLFUNC_NO_ERROR(dll.handle, dll.reload_global_data, "reload_global_data");

    ret_code = 0;
out:
    if (!flag) {
        BOOT_LOG(ret_code, "dlopen %s", file_name);
    } else {
        NOTI_LOG("dlopen %s %s", file_name, (ret_code) ? "FAIL" : "OK");
        return 0;
    }
}

void unregister_data_plugin()
{
    if (dll.data_handle != NULL){
        dlclose(dll.data_handle);
        dll.data_handle = NULL;
    }
}

void unregister_plugin()
{
    if (dll.handle != NULL){
        dlclose(dll.handle);
        dll.handle = NULL;
    }
}

