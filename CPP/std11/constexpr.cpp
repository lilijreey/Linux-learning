/*
 *  Description: 常量表达式
 */

#include <stdio.h>


#define	CPP_0X11			/*  */

#ifndef CPP_0X11 
inline int return4() 
{return 4; }

int main() {
    //g++ 的实现中不用constexper, std=c++0x  也可以通过编译
    int arry[return4()] ;
    return 0 ;

}

#else

//EE1 允许函数返回一个常理，在编译器执行
//将一个函数声明为constexpr 有非常严格的条件
//1.函数返回类型不能是void
//2.必须是return expr
//3.expr必须是编译期know，常理
//4.必须在被使用之前定义
constexpr inline int return5() 
{return 5; }

//EE 2
//变量也可以声明为constexpr
//在C++98中const 常量只能是整形或枚举型，
//而用constexpr 
int main() {
    constexpr double IP = 3.141592653;
    const int FIVE=return5();
    return 0 ;
}


#endif
