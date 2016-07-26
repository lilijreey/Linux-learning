/*
 * =====================================================================================
 *
 *       Filename:  printf.c
 *
 *    Description:  
 *
 *        Created:  12/25/2012 12:31:45 PM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */

/*
 * printf 对typedef 的打印，
 * 1.pid_t thread_t 使用%u
 *
 * 2.size_t ssize_t 使用%zu
 */

#include <stdio.h>
#include <stdarg.h>

/*
 * vsnprintf return
 */

void xx(const char *fmt, ...)
{
    va_list ap;
    char buf[20];
    int n;

    va_start(ap, fmt);
    //vsnprintf 返回fmt 展开后写入的数量, 不包括\0
    //但是vsnprint 会在写入后添加一个\0, 如果buf狗大
    //vsnprint 知会写入 size-1 个字符。 
    n = vsnprintf(buf, 1, fmt, ap);
    va_end(ap);
    printf("N:%d|%s\n", n, buf);
}

int
main(int argc, char *argv[])
{
    /*xx("%s", "01234567890");*/
    /*xx("%d|012345", 24);*/

    /*printf("%zd\n", 323);*/

    //格式化 指定字符串最大长度
    //*.* <最小长度>.<最大长度>
    printf("%*.*s\n", 5, 8, "0123456789");

    //.* .<最大长度>
    printf("%.*s\n", 3, "aaaaaa");

    //值定输出长度4, 不足的用空格补齐
    printf("%4s\n", '3', "aa");

    return 0;
}				/* ----------  end of function main  ---------- */
