/**
 * @file     errno_t.h
 *           
 *
 * @author   lili <lilijreey@gmail.com>
 * @date     04/03/2014 04:03:34 PM
 *
 * Linux 系统定义的errno 在 /usr/include/asm-generic/errno-base.h
 * /usr/include/asm-generic/errno.h 中 0-132
 * 
 * EE 系统定义的errnor string 在
 *  全局 sys_errlist[] 中定义
 *  最大的errno值为 sys_nerr -1
 *  Man 3 sys_nerr
 *  
 */


#include <stdio.h>

//errno -> string
// strerror strerror_t
// 但是从Nginx源码来看作者这两个函数也是有问题的所以自己dup的一份自己用 
/*
 * The strerror() messages are copied because:
 *
 * 1) strerror() and strerror_r() functions are not Async-Signal-Safe,
 *    therefore, they cannot be used in signal handlers;
 *
 * 2) a direct sys_errlist[] array may be used instead of these functions,
 *    but Linux linker warns about its usage:
 *
 * warning: `sys_errlist' is deprecated; use `strerror' or `strerror_r' instead
 * warning: `sys_nerr' is deprecated; use `strerror' or `strerror_r' instead
 *
 *    causing false bug reports.
 */


int
main(int argc, char *argv[])
{
    int i;
    int max_errno = sys_nerr -1;
    for (; i < max_errno; ++i) {
        if (sys_errlist[i] != 0)
           printf("%d :%s\n",i, sys_errlist[i]);
    }
  printf("max errno %d\n", max_errno);
  return 0;
}				/* ----------  end of function main  ---------- */
