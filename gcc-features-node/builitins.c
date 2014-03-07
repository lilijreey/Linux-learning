/*
 * =====================================================================================
 *
 *       Filename:  builitins.c
 *
 *    Description: GCC __builtin系列函数, __builtin系列函数主要用于，
 *                 不同体系提供的高级特性，通常不同的体系__builtin 
 *                 不同。
 *
 *        Created:  07/13/2012 02:05:04 PM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */

#include <stdlib.h>

/// @brief __builtin_return_address(0) get the return adress of a func
// 即，函数结束时控制流所在的obj地址
// 参数指定了，调用层次，0为本函数的返回的地址，1表示调用当前函数的函数
// 的返回地址....

/// @brief __builtin_expect(long exp, long c) 帮助编译器优化分支预测.
// exp 是一个将计算的表达式，c是期望的返回之(0,1)
// e.g
void (struct obj *d)
    if (obj == NULL) return; //d impossible is NULL
    else
      //  do something
    //use __builtin_expect
    if (__builtin_expect(obj == NULL, 0)) //unlikely is NULL
            //do somthing
    //or
    if (__builtin_expect(obj != NULL, 1)) //likely not NULL
            //do somthing
}

//  内核 定义了两个宏，来标记代码中的可能和不可能
/// @brief  define __likely(x)   __builtin_expect(!!(x), 1)
/// @brief  define __unlikely(x) __builtin_expect(!!(x), 0)
// 使用双重否定！！的理由：
// 1使得宏可以用于隐式转换成真值指针
// 2大于0的真值.e.g exp =4 但是4 != 1,可是 !!4 =1

/// @brief __builitin_constant_p(EXP) .
/// @return 1 EXP 是常数 0 不是.
///     用于判断EXP在编译器是不是常数 


