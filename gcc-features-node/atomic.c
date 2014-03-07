/*
 * =====================================================================================
 *
 *       Filename:  atomic.c
 *
 *    Description:  gcc 提供的原子内存操作
 *
 *        Created:  04/13/2013 01:02:51 PM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */

/*
 * CAS compare_and_swap/3 比较并更新(原子的)
 * if (*ptr == oldval)
 *     *ptr = newval
 *
 *  两种形式
 *  @return: 返回 true 说明更新了ptr
 *  bool __sync_bool_compare_and_swap (type *ptr, type oldval type newval, ...)
 *
 *  @return: 返回 ptr 的原始值
    type __sync_val_compare_and_swap (type *ptr, type oldval type newval, ...)
    如果*ptr 等于oldval 则更新*ptr 为newval
 *
 * + Type GCC 允许任何 长的为 1,2,4,8 integer pointer 的原子内存操作
 *           所有的这些操作都是内存屏障的
 *
// 返回操作之前的值
//type __sync_fetch_and_add (type *ptr, type value, ...)
//type __sync_fetch_and_sub (type *ptr, type value, ...)
//type __sync_fetch_and_or (type *ptr, type value, ...)
//type __sync_fetch_and_and (type *ptr, type value, ...)
//type __sync_fetch_and_xor (type *ptr, type value, ...)
//type __sync_fetch_and_nand (type *ptr, type value, ...)
//
// 返回操作之后的值
//type __sync_add_and_fetch (type *ptr, type value, ...)
//type __sync_sub_and_fetch (type *ptr, type value, ...)
//type __sync_or_and_fetch (type *ptr, type value, ...)
//type __sync_and_and_fetch (type *ptr, type value, ...)
//type __sync_xor_and_fetch (type *ptr, type value, ...)
//type __sync_nand_and_fetch (type *ptr, type value, ...)
// type 是 + Type
 *
 * EE 发出一个 full memory barrier 内存屏障
 *  __sync_synchronize ()
 *
 *  memory barrier有几种类型：
    acquire barrier : 不允许将barrier之后的内存读取指令移到barrier之前（linux kernel中的wmb()）。
    release barrier : 不允许将barrier之前的内存读取指令移到barrier之后 (linux kernel中的rmb())。
    full barrier    : 以上两种barrier的合集(linux kernel中的mb())。

    EE 把变量设置为指定值
    返回 ptr 以前的值
type __sync_lock_test_and_set (type *ptr, type value, ...)

 */
#include <stdlib.h>
#include <stdio.h>
//
int main()
{
    volatile int a = 3;

    // atomic_add int
    printf("%d\n",__sync_fetch_and_add(&a, 5));
    printf("%d\n",__sync_sub_and_fetch(&a, 5));

   if (__sync_bool_compare_and_swap(&a, 3, 11)) 
       printf("true\n");

   int b =__sync_lock_test_and_set (&a, 100);

   printf("%d\n", b);
    return 0;
}


