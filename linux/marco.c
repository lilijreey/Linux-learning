/**
 * @file     marco.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     2015年06月24日 18时21分20秒
 *
 */

#include <stdio.h>
#define	AA 3

//#define AA 4 ## EE 无法重定义一个一定定义的宏

#define BB AA

//#undef AA //## undef AA 后 AA 会从3，回复到4吗? Ans. 不会


int
main(int argc, char *argv[])
{
    int i = BB;
    int b = AA;
    printf("%d, %d\n", i, b);
    return 0;
}				/* ----------  end of function main  ---------- */


