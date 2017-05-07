/**
 * @file     chrono.cpp
 *  C11 支持良种的时间机制， 一种是C-style |  一种是chrono
 *  chrono library
 * 
 *  The chrono library defines three main types 
 *  (durations, clocks, and time points) as well as utility 
 *  functions and common typedefs.
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     05/17/2013 11:52:06 AM
 *
 */


/// std::chrono:system_clock 
//  系统即实时 wall clock

/// <ratio> 分数 提供编译期的算术支持,都是常量
//template<
//    std::intmax_t Num, 分子
//    std::intmax_t DENOM = 1 //分母
//> class ratio; 提供任意精度的数字表示
//  
// Member Objects
/// num 得到分子
/// den 得到分母

///duration

///几个常用的时间常量
///std::chrono::nanoseconds 	duration<, std::nano>
///std::chrono::microseconds 	duration<, std::micro>
///std::chrono::milliseconds 	duration<, std::milli>
///std::chrono::seconds 	duration<>
///std::chrono::minutes 	duration<>
///std::chrono::hours 	duration<std::ratio<3600>>
#include <stdio.h>
#include <chrono>
int main()
{
  std::ratio<3, 4> fe;
  printf("num:%ld, den:%ld\n", fe.num, fe.den);

}
