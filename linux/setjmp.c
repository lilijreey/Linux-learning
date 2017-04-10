/**
 * @file     setjmp.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     03/29/2014 07:55:36 PM
 *
 */
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <setjmp.h>
/*
 * 只能是运行到setjup函数后再执行的longjmp才能跳转到setjmp
 * 跳转到setjmp后setjmp返回非0 值可以用来检测是正常执行到
 * setjmp还是longjmp导致的setjmp返回
 * 
 * setjmp/1
 * longjmp/2(jmp_buf, val) val 是设置setjmp 的返回值的，用来处理不同
 * 类型的异常
 */

jmp_buf bf;

void oo()
{
    int i;

    while (1)
    {
        scanf("%d", &i);
        if (i == 0) {
            printf("jump to\n");
            longjmp(bf, 1);
            printf("jump next \n");
        }
        printf("Enter %d\n", i);
    }

}

void xx() {
//    i = setjmp(bf); //如果多次设置，longjmp之会返回到最近一次设置的地方
//    printf("Level 1:%d\n", i);

    if (0 == setjmp(bf)) {
        printf("first setjmp\n");
    } else { //handle longjmp
        printf("handle longjmp\n");
    }
    printf("Level 2\n");
    printf("xx\n");
    oo();
}

// EE longjmp 后对变量的更改
void cc()
{
    int i = 0;
    int j = 0;
    int k = 0;
    if (0 == setjmp(bf)) {
        printf("first setjmp i:%d, j:%d, k:%d\n", i,j,k);
    } else { //handle longjmp
        printf("jump i:%d, j:%d, k:%d\n", i,j,k); //改变会保持
        sleep(1);
    }

    j++;
    k++;
    longjmp(bf, 1);
}

int
main(int argc, char *argv[])
{

    printf("Start\n");

    cc();
//    xx();

    return 0;
}				/* ----------  end of function main  ---------- */
