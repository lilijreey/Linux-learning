/*
 *  Description:  c++ x11 中其他的一些新特性和函数
 */
#include <stdio.h>

///auto 推倒类型, 注意推倒时的类型退化
//函数的参数类型，不能声明为auto

///1. 会自动把 引用去掉
//void f(int & a) {
// auto b =a; //type b is int
//}

///auto  推倒 {x,y,z} 会得到std::initalize_list<T> 类型

// int  m;
//const auto * p2 = &m; const int *
//const auto & i2 = m; const int &
//
//
 //int a = 3;
 //auto b = &a;
 //std::cout << typeid(b).name() << std::endl;

 //int &c = a;
 //auto d = c; //d is int
 //std::cout << typeid(d).name() << std::endl;

 //auto &d1 = c; d1 is int& reference c
 //std::cout << typeid(d1).name() << std::endl;
//
///auto&& a1= m; 由于x是左值，所以 a1 type is int&
///auto&& a2= std::move(m); a2 type is int&&


///auto 推倒类型时，会忽略const和volatile 退化
//e.g. const std::list<int> lli;
// auto v1 = l1 ; //type v1 = std::list<int>
// auto &v2 = l1 ; //type v2 = const std::list<int>&
// int data[3];
// auto d1 = data; //type d1= int*
// auto &d2 = data; //type d2= int(&)[3]

/// auto v1(exp);会使用v1的直接初始化
/// auto v2 = exp;会使用v2的copy 初始化 


///decltype
//compile-time 得到一个表达式的返回类型，主要用于泛型编程的类型不可
//声明问题
// 返回类型后置

template <typename T, typename U>
auto add(T t, U u) -> decltype(t + u) {
  return t+u;
}

///EE std::result_of 
// 在编译期推到出一个function


///std::addressof
//efined in <memory> header
//去的一个对象的实际地址，就算&被重载
//e.g addressof(x);


///range-base for loop
//for (element : body)
/// 1 essentially equie to 
//{
//  auto&& b = body;
//  for(auto it=begin(b), e = end(b); b != e; ++b) {
//   element = *b;
//   do...
//  }
//}
/// range-base for 的作用对象
// 1. All STL 
// 2. std::initailiz_list
// 3. array 
// 4. user type 只用提供了begin, end, 和迭代骑的++操作都可以使用range-base for
///注意在range-base for 中间操作容器时不能，添加和删除容器元素
//应为 range-base 展开的本质 sse [essentially equie to]

void vrf()
{
  int arr[] = {1,2,3,4,5,6,7,8,9, 0};
  for (int e : arr)
  printf("%d ",e);

}

//EE remove_reference
//EE is_reference
//EE is_lvalue_reference
//EE is_rvalue_reference
//EE integral_constant
//EE is_same
//... type traits

///Unicode support
// utf-8 string 字面直
// e.g. u8"string哈哈"
///2 可以通过\uXXXXX 来指定字符
/// u'xxx' as char16_t using UTF-16
/// U'xxx' as char32_t using UTF-32
//std::u16string std::u32string == std::basic_string<char_xx_t> 
//
///3 Raw string literals 使用 R"(str)"
//这里的"( , )"是delimiters 界定符， 这个符号可以自定义
//对string中的转义字符都不做转义处理
// char 8 str =R"(\n\n)";
// 对于写正则表达是的pattern时经常用的

void uni()
{
  printf(u8"jofe你哈:\u2620\n");
  printf("Raw str:%s\n", R"(\n\n)");
  //自定义Raw string界定符
  printf("Raw str:%s\n", R"!(\t\n\t\a)!");

}

int main() 
{
//  vrf();
 uni();

    
	return 0 ;
}

///EE namespace
// namespace bo = boost;
