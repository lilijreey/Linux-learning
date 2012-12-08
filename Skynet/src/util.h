#ifndef SKYNET_SERVER_UTIL_H_
#define SKYNET_SERVER_UTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef  likely
#undef  likely
#endif
#define likely(x)  __builtin_expect(!!(x), 1)

#ifdef  unlikely
#undef  unlikely
#endif
#define unlikely(x)  __builtin_expect(!!(x), 0)

int log_init_ex(const char* dir, log_lvl_t lvl, uint32_t size, int maxfiles, const char* pre_name, uint32_t logtime);
int pipe_create(int pipe_handles[2]);

#ifdef __cplusplus
}
#endif

#endif // SKYNET_SERVER_UTIL_H_
