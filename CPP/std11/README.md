C++11 的新特性
===============================

1. auto 类型自动推导。  see othersFeature.cpp
2. static-assret  编译期断言
3. Unicode support see [othersFeature.cpp]
4. nullptr
5. constexpr
6. variable arugments template 可变参数模版
7. 统一初始化 initizaser-list
7.1 成员变量就地初始化，就是在class 定义中就可以使用= or {} 初始化
  e.g. struct xx{int = 1; double b {1.3};};
8. move semantics 移动语义
      see rightValueReference.cpp
9. rvalue reference
10. range-base for loop [see othersFeature.cpp]
11. control defualt function =delete;
12. attributes 属性
13. decltype/auto
14. raw string
    R"delimiter(raw_string)delimiter"
using 类型别名定义
using u16 = short;

15. algorithms 有添加了几种通用算法
         all_of any_of none_of one_of ..
         see algorithms.cpp


16. noexcept         
     98 中一个函数后面可以声明次函数可能抛出的异常
      e.g.. void xxx() throw(int, double) {...}
      e.g.. void xxx() throw() {...} // 不抛出异常
       不过这一特性很少使用，在11 中已经被废弃
      如果一个函数不会抛出异常则可以使用 noexcept 声明
      如果一个noexcept 修饰的函数抛出了异常则编译器可能会
      直接调用std::terminate() 函数来终止程序
      e.g. viod xx() noexcept;
      e.g. viod xx() noexcept(常量表达式)
              常量表达式的结果会被转换成一个bool 值，如果为true
              则说明不会，如果为false则说明可能会抛出异常
     常用于模板
     11 中类的析构函数默认都是noexcept(ture)的, 除非自己修改，或者
     基类的析构不是nexecept(ture)的

17. 非静态成员的 sizeof
     sizeof(Class::name)

18. 扩展的friend 语法
      不需要在写 friend class xx
        friend xx
      可以为类模板声明友元了
        template <typename T> class People {
             friend T;
        }
        friend 内建类型都会被忽略


19.  继承构造

```
class _A
public:
    _A( int _InInt ) {;}
    _A( double _InDouble, int _InInt ) {;}
   _A( float _InFloat, int _InInt, const char* _Char ) {;}
    };
 

class _B : public _A
    {
public:
   using _A::_A;            // 使用_A中的构造函数
   可以把_A 所有的构造函数都继承了 ,如果派生类新添加了成员
   则使用继承构造会导致这些成员不会被初始化,最好用在值添加接口的
   派生类上,
   如果使用的继承构造,就不会生成默认的构造函数

   或者配合 就地初始化
    // Somthing...
    virtual void _ExtraInterface() {;}
    };
```

New Lib
1. array
2. turpe
3. unordered-map
4. unordered-set
5. thread
6. Type traits in <type_traits> header
7. Smart protected:inters in <memory> header
8. Date and time utilies <chrono>
9. hash in <functional> header
10. atomic operator <atomic> header
11. regular expressions <regex> header
12. Data and time <chrono> header
13. forward-list
14. emplace-op 不会产生copy后move动作直接在容器上产生对象
     std::vector<Obj> fef;
     fef.push_back(Obj(123)); //这个可能会产生 copy 后move操作，从Obj到vector中
     fef.emplace_back(123) 之产生对象一次
      
      Emplace takes the arguments necessary to construct an object in place,
       whereas insert takes (a reference to) an object.
        struct Foo
        {
          Foo(int n, double x);
        };

        std::vector<Foo> v;
        v.emplace(someIterator, 42, 3.1416);
        v.insert(someIterator, Foo(42, 3.1416));

