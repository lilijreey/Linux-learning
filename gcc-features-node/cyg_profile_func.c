/*
 * =====================================================================================
 *
 *       Filename:  cyg_profile_func.c
 *
 *    Description:  gcc 内置的追踪函数调用的回调函数
 *
 * =====================================================================================
 */

//EE -finstrument-functions  ...
//  必须在gcc 编译时指定 -finstrument-functions 选项才能开启
//  会在每次进入和离开函数时，调用名为__cyg_profile_func_enter
//                             和__cyg_profile_func_exit函数
//
//
//void __attribute__((__no_instrument_function__))
// __cyg_profile_func_enter(void *this_func
//                          void *call_site);
//
// 会在刚进入一个func时被调用
//
//void __attribute__((__no_instrument_function__))
//__cyg_profile_func_exit(void *this_func,
//                       (void *call_site);
// 
// 会在刚即将离开一个func时被调用
//
// +  this_func 参数是当前函数的地址
// +  __no_instrument_function__ 加上这个属性是为了对__cyg_profile_func_* 
//        自身调用时的无限递归调用
//     
// 用途： 主要用来调试，分析程序
//        e.g. 有了func的地址就可以使用addr2liens 等工具把地址转换为
//             对应的函数名称和文件名，从而可以得到函数的调用顺序

#include <stdio.h>
#define DUMP(func, call) \
    printf("%s: func = %p, called by = %p\n", __FUNCTION__, func, call)

void __attribute__((__no_instrument_function__))
__cyg_profile_func_enter(void *this_func, void *call_site)
{
    DUMP(this_func, call_site);
}
void __attribute__((__no_instrument_function__))
__cyg_profile_func_exit(void *this_func, void *call_site)
{
    DUMP(this_func, call_site);
}

int main()
{
    puts("Hello World!");
    return 0;
}
//输出结果, 两回的this_call和callsit 都是相同的
//__cyg_profile_func_enter: func = 0x400616, called by = 0x7f16d3c0776d
//Hello World!
//__cyg_profile_func_exit:  func = 0x400616, called by = 0x7f16d3c0776d
