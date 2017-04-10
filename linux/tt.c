/**
 * @file     tt.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     03/06/2014 01:57:19 PM
 *
 */
#include <stdio.h>

struct oo
{
    int a;
    int b;
    long c;
    char d;
    char e;
};

int
main(int argc, char *argv[])
{
    printf("offsetof a:%lu\n", __builtin_offsetof(struct oo, a));
    printf("offsetof b:%lu\n", __builtin_offsetof(struct oo, b));
    printf("offsetof e:%lu\n", __builtin_offsetof(struct oo, e));
    return 0;
}				/* ----------  end of function main  ---------- */
