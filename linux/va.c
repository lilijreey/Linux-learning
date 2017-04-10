/**
 * @file     xx.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     02/10/2014 05:19:51 PM
 *
 */
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <stdarg.h>


#define	_va_start(ap, last)	ap = &last
#define _M_INTSIZEOF(size)   ((size + sizeof(int) - 1) & ~(sizeof(int) - 1))
#define _va_arg(ap, size) (ap -= _M_INTSIZEOF(size))

/*
 * Gcc va_list 实现作为一个struct
 * 
 */

void xx(int n, size_t size, ...)
{
    void * vo;
    void *ptr;
//  void *arg;
  char data[size];
  size_t up_size = _M_INTSIZEOF(size);

  va_list vl;
  va_start(vl, size);

//    vo = (void*) &size - _M_INTSIZEOF(sizeof(size_t));

  _va_start(vo, size);
  
//  va_start(lp, size);
//  vo = lp;
//
    ptr = _va_arg(vo, up_size);
    printf("ptr:%p\n", ptr);
    memcpy(data, ptr, size);

    ptr = _va_arg(vo, up_size);
    memcpy(data, ptr, size);
    printf("ptr:%p\n", ptr);

    ptr = _va_arg(vo, up_size);
    memcpy(data, ptr, size);
    printf("ptr:%p\n", ptr);

    printf("size_t %lu\n",sizeof(size_t));


//    i = va_arg(lp, int);
//    printf("i:%d\n",i);
//    i = va_arg(lp, int);
//    printf("i:%d\n",i);
//    i = va_arg(lp, int);
//    printf("i:%d\n",i);


//
//    c = &i;
//    c+=32;
//    arg = ()
//    size_t up_size = _M_INTSIZEOF(size);
//    (uintptr_t) lp +=up_size;

//    printf("first lp :%p\n", lp);
//    for (i=0; i < n; ++i) {
//        e = _va_arg(lp, up_size);
//        memcpy(data, e, size);
//    }

}

void yy(int i, int y, int m, int n)
{
    void *vo = (void*) &i;
    void *ptr;
    printf("vo:%p\n", vo);
    ptr = _va_arg(vo, sizeof(int));
    printf("ptr:%p\n", ptr);
    ptr = _va_arg(vo, sizeof(int));
    printf("ptr:%p\n",ptr);
}

int
main(int argc, char *argv[])
{
    xx(3, 1, 0xff);

    return 0;
}				/* ----------  end of function main  ---------- */

