/**
 * @file     variant_t.cpp
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     04/23/2014 09:52:26 AM
 * 
 * 一个union container ，里面的元素可以是任意的类型，但是是单值
 *  元素类型 的要求是可拷贝构造，息构不抛出异常
 *  
 */


#include <string>

#include <boost/variant.hpp>
#include <iostream>

//union {
//  int i;
//  std::string s; //bad just POD type
//} u;

/// static_visitor 用来访问variant 必须继承
class my_visitor : public boost::static_visitor<> //for void return type
{
 public:
  void operator()(int i) const 
  { std::cout << "is is a intger " << i << std::endl;}

  void operator()(const std::string &s) const
  { std::cout << "is is a string: " << s << std::endl;}

};

class two_generic : public boost::static_visitor<> //for void return type
{
 public:
  void operator()(int &i) const 
  { i *= 2;}

  void operator()(std::string &s) const
  { s += s;}

};


int main() {

  /// 构造variant 
 //  无参的构造函数，使用第一个模版类型的默认构造函数初始化,
 //  有参数的，使用对应类型的匹配构造参数
  boost::variant<int, std::string>x;
  std::cout <<  "x: " << x << std::endl;
 
  boost::variant<int, std::string> u("hello, world");
  u = 32;
  std::cout << u << std::endl;

  std::cout << u << std::endl;

  ///member func
  //empty/0 -> bool test obj has a 总是返回false 只是为了兼容 any

  //type
  std::cout << "x type() : " << x.type().name() << std::endl;
//assert(x.type() == typeid(int))

  //which/0 -> int 返回当前值类型在模版参数中的索引 从0开始
  std::cout << "x type index is " << x.which() << std::endl;

  ///EE 当有一个variant 对象时我们不知道他里面到底是那个类型，
  //我们需要使用 static_visitor 模式来访问他
  
  boost::apply_visitor(my_visitor(), u);

  boost::apply_visitor(two_generic(), u);
  std::cout << u << std::endl;

  ///访问 variant 的值
  //使用boost中个 get函数
//  int &o = boost::get<int>(x); //  如果类型不正确会 抛出 boost::bad_get 一场
//  int *pi = boost::get<int*>(x); // 如果类型不正确会返回 nullptr
//  pi= NULL;

  //更好的访问元素使用 visitor 模式
  //继承 static_visitor ,然后给重在 operator()
  //
  //访问 对象应用到 variant 需要使用boost::apply_visitor 来调用
  //
  //

}
