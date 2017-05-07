/**
 * @file     overloadOp.cpp
 *           重载操作符
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     06/03/2013 04:42:25 PM
 *
 */


#include <stdio.h>

struct As
{
  int i;

  /// 重载类型转换操作符
  operator int() 
  {
    printf("call operator int()\n");
    return i;
  }

  template<typename R>
  R get();
  
};

template<>
int As::get()
{
  //会调用 operator int()
  return *this;
}

int main() {
  As a;
  a.i=32;
  int i = a;
  printf("i:%d\n", i);

  i = a.get<int>();

}
