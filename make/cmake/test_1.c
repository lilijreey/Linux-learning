/**
 * @file     test_1.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     2015年05月26日 20时20分30秒
 *
 */
#include <stdio.h>

//使用第三方库 头文件和lib
#include "other1/other.h"

int
main(int argc, char *argv[])
{
    #ifdef LINUX # defined by cmake
    printf("cmake define LINUX get\n");
    #endif
    printf("%s\n OTHER_VER %d", argv[0], OTHER_VER);
    user_other();
    return 0;
}				/* ----------  end of function main  ---------- */

