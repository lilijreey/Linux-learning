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

// {{{ CPU about

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


//set vim: foldmethod=marker
