/**
 * @file     atoi_t.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     04/04/2014 09:57:07 AM
 *
 */
#include <stdlib.h>

#include <stdio.h>


int
main(int argc, char *argv[])
{
    // -> 3232
    printf("%d\n", atoi("3232a3"));
    char *endptr;

    // 3232 -> 3232
    // 0x11 -> 17 endptr -> NUL
    // 32aaa33 -> 32 endptr -> a
    // 

    // -> long 可以检测到无效的数字
    // base参数 如果为零， 则可以识别0x前缀
    // endptr 是返回最后一个无效数字的，可以用来判断
    // 给定的字符串中是否有无效的数字
    // 
    printf("strtol 3232:%ld\n", strtol("3232", NULL, 0));
    printf("strtol 3232ab3:%ld\n", strtol("3232ab3", &endptr, 0));
    if (*endptr != '\0') {
        printf("bad str\n");
    }

    printf("strtol 0x11:%ld\n", strtol("0x11", NULL, 0));
    printf("strtol abe:%ld\n", strtol("abe", NULL, 0));

    return 0;
}				/* ----------  end of function main  ---------- */
