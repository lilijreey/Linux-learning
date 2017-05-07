/*
 *  Description: 可变模版参数
 */

#include <stdio.h>
#include <iostream>

//EE1 使用 typename... 声明可变模版参数
//EE2 使用 sizeof... 得到参数的个数

template <typename... Types>
void f(Types... args) {
    printf ( "Types count is:%u\n", sizeof...(args)); 
}

//EE3 如何使用参数， 不能直接使用参数，只能递归的取出参数
//？
template <typename T, typename... Types>
void s(T t, Types... args) {
    printf ( "Types count is:%u\n", sizeof...(args)); 
    std::cout << t << std::endl ; 
    f(args...);
}

//不存在模版函数指针，以为模版只是模版，不是类型也就没有对象
//typedef template <typename... Types>
//void (*x)(Types...);

int main() {
    f(1, 0.3);
    s("ok", 323, 121);

    
	return 0 ;
}


