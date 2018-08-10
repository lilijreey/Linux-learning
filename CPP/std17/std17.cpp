/**
 * @file     std17.cpp
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     08/10/2018 10:26:02 PM
 *
 */
#include <stdio.h>

//EE 解构绑定
//
// pair<int, string> pp{33, "ooo"};
// auto [num, str] = pp;


// EE constexpr if
// 编译期if 
// 使用, 和if/esle类型，只是表达式必须可以在编译期求知
// if constexptr(exp 可在编译期求知)
//   {
//   } else{
//   }
//
//  if constexpr (exp) {
//  } else ifconstexpr (exp) {
//
//  } else {}
//
//
// 用途, 预处理#if/else的一种替代

enum {
  DEV_TYPE,
  TEST_TYPE,
  PROD_TYPE,
  
};
constexpr int env_type = DEV_TYPE;

struct AA{
  int id;
#if dev_env
  fjeof
#endif
  //不能用在这里
  //if constexpr (env_type == DEV_TYPE)
  //{
  //  int dbgInfo;
  //}


};

template <class T>
void add(T t)
{
  if constexpr (sizeof(T) == sizeof(int))
  {
    printf("sizeof int");
  }
  else {
    printf("sizeof other");

  }
}

//EE 内联变量， 用来在头文件定义全局变量，而不会造成重定义
// inline int env=3;

int main()
{
  add(3);
}
