/*
 * =====================================================================================
 *
 *       Filename:  c11-type_traits.cpp
 *
 *    Description:  
 *
 *        Created:  12/28/2012 02:38:49 PM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <cassert>
#include <type_traits>

//is_same 判断两个类型是否相同
//通过is_same 来对不同模板参数执行不同的代码
struct A{};
struct B{};

template <typename T>
void pp(int i) 
{ //HACK 还是不太好，使用编译器if
  if (std::is_same<T, A>::value)
    printf("T is A\n");
  else if (std::is_same<T, B>::value)
    printf("T is B\n");
  else
    assert(false);

  printf("%d\n", i);
}


int main() {
  pp<A>(3);
  return 0;

}

