/*
 * =====================================================================================
 *
 *       Filename:  sysconf.c
 *
 *    Description:  sysconf 函数 include in <unistd.h>
 *
 *        Created:  03/06/2013 11:13:49 AM
 * =====================================================================================
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// {{{ CPU about

#if 0
// _SC_NPROCESSORS_ONLN // 取得机器当前有效（可用)的CPU个数
// _SC_NPROCESSORS_CONF // 取得机器配置时CPU 的个数
int main() {
  long l = sysconf(_SC_NPROCESSORS_ONLN);
  printf("available cpu count:%ld\n", l);
  l = sysconf(_SC_NPROCESSORS_CONF);
  printf("config cpu count:%ld\n", l);
  return 0;
}
//}}}

#endif


//process default max open fd value? ulimit -n /ulimit -a (show all)
#include <sys/time.h>
#include <sys/resource.h>

//ulimit -aS 察看当前 shell 的Soft资源限制
//ulimit -aH 察看当前 shell 的Hard资源限制
//ulimit -n 330 修改当前shell的 NOFILE soft 限制为300， 所有在修改后从shell创建的
//processs 也会继承他
 
// 系统默认的资源限制数值可以在 /etc/security/limits.conf
// 中设置，比如设置nofile
// soft nofile 5000
// hard nofile 10000
// 
// 修改hard 设置，
// 
// 整个系统的最大nofile /proc/sys/fs/file-max
// 
// Qus. fork 后继承 limit 吗
int main()
{
    struct rlimit  def;
    if ((getrlimit(RLIMIT_NOFILE, &def)) == -1) {
        perror("getrlimit failed");

        return 0;
    }

    //1024, 4096
    printf("maximum fd cur:%lld max:%lld\n", (long long)def.rlim_cur, (long long)def.rlim_max);

    return 0;
}
