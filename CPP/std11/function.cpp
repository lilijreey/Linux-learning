/**
 * @file     function.cpp
 *
 * @brief    <functional> 中 function 函数对象学习笔记
 *
 * @author   lili  lilijreey@gmail.com
 * @date     05/01/2013 10:32:44 PM
 *
 */

// +function是什么？
//   function 是一个模版类，是一个通用的函数包装器 
//   function 的所有特化类都是仿函数类，
//   function 的实例可以是任何可调用的对象(函数指针，其他的仿函数，bind exp,
//   lamda 等)
//
//   bind
//
// +为什么需要function?
//  提供了对象级委托(delegate)手段
//   比函数指针更加灵活，因为可以function可以从任何可调用的对象构造，也可
//
// +function能做什么?
//  用来取代虚函数,比虚函数更强大，提供对象级别的委托，不同实例动态设置自己的处理方式
//
//
// +function最佳实践
//
// function的创建
// 函数的类型<R(xxx)> R 是返回类型，


///+ bind 接受任何可以调用的对象，返回一个function object
//  std::bind(callableEntify， Arg1,arg2..)
///+ 所有传递给 bind  参数都会coyp到bind返回的对象中
///+ Binding Non-static Mumber Function
//  需要在参数前先传递"this" bind支持3中类型的this
//   * Object copy Object to bind
//     A bh;
//     std::bind(&A::call, bh, _1); 
//     这种会coyp
//     对象，对不不可copy对象不能使用，对用copy开销开的对象不推荐使用
//   * Pointer
//     std::bind(&A::call, &bh, _1); 
//      传递在调用bind的返回对象时 object 必须存在,因为你传递的是指针
//   * Smart Ponter
//      std::shared_ptr<A> sp;
//      std::bind(&A::call, sp, _1); 
//     
///+ std::placeholders 
//     bind返回对象的参数站位符， 从_1 开始，
///     std::placeholders 使用的是引用传递
//  auto x = bind(f, _3, _2, _1)  // 创造一个新的std::functon
//               他的参数位置和f相反
//  auto x2 = bind(f, _1, 2.2, 'c');
//
///+ 无法使用bind 来绑定重载的函数， 必须制定哪个版本的重载函数
///+ Lambda VS Bind
//  lambda 也返回function Object
//  lambda 通常都比bind 更清晰移动
//  lambda 通常会生成更好的code
//  bind 不会内联
//  lambda 允许内联


#include <functional>
#include <stdio.h>

void f1() {printf("call f1\n");}
void f2(int i) {printf("call f2 :%d\n", i);}

void f3(int i, int c, int o)
{
  printf("f3 i:%d 2:%d 3:%d\n", i, c, o);
}

struct A {
  int memf()  {
    printf("A:memf i:%d\n", i);
    return i;
  }
  static void staticFunc(int i)
  {
    printf("A::static func i%d\n",i);
  }

  int i;
};

class B {
 public:
  void init(const std::function<void(int)> f)
  {_ff = f;}

  void call(int i)
  {
    _ff(i);
  }

  void setFo(std::function<int(void)> &fo)
  {
    _fo = fo;
  }

  int co()
  {
    return _fo();
  }
 private:
      std::function<void(int)> _ff;
      std::function<int(void)> _fo;
};

//吧这个帮给 bind
struct FunO1
{
  FunO1(int i)
      :_i(i)
  {}

  int operator()() {
    printf("FunO1 i:%d\n", _i);
    return _i;
  }

  int _i;
};

struct FunO2
{
  FunO2(int i)
      :_i(i)
  {}

  int operator()(void) {
    printf("FunO2 i:%d\n", _i*2);
    return _i;
  }

  int _i;
};

struct FunO3
{
  void operator()(int i) {
    printf("FunO3 i:%d\n", i*2);
  }

};

int main()
{
  ///@note creat empty function
  std::function<void()> emp;
  //emp 在没复制以前不可以调用
  //ld error can not find ff1 reference
  emp = f1;
  emp(); 

  ///@note creat from common function
  std::function<void()> ff1(f1);
//  std::function<void()> ff1 = f1;
  ff1(); 

  ///@note creat from class member func
  //对用成员函数需要声明期隐含的this类型为第一个参数
  std::function<int(A&)> fmemf(&A::memf);
  A a;
  a.i=10;
  fmemf(a);

  ///@note creat from lembda exp
  std::function<void(int)> flem = [](int i){printf("lembda:%d\n", i);};
  flem(3);
  //写一个返回一个闭包的function

  ///@把GCC4.6.4 搞挂了 4.6 还不支持。 4.8 oK
//  std::function<std::function<int()>(int)> rclouse = [](int i) {
//    return [i](){++i;};
//  };

  //得到一个初始化i=3 的add function
//  std::function<int()> seed3 = rclouse(3);
//  printf("%d\n", seed3());

  ///@note create from other std::function
  //std::function<void()> xx = std::bind(xx)

  ///@note use class 
  B b;
  b.init(f2);
  b.call(3);
  printf("sizefo classB:%lu\n", sizeof(b));

  ///@对没有赋值的 std::function<void(int)> f 调用是什么结果
  // 会抛出异常 std::bad_function_call
  //  std::function<void(void)> vf;
  //  vf();

  ///EE 用一个class作为function
  FunO1 fo = FunO1(3);
  FunO2 fo2 = FunO2(3);
  auto fucO = std::bind(fo);
  fucO();
  
  ///EE 把一个bind 放函数传给一个 对象的成员,
  //这样可以不同类型都可以复用这个仿函数, 如果是个仿函数直接传入对象
  //需要先使用function 如果直接写入参数编译不过
  std::function<int()> f1 = std::bind(fo);
  b.setFo(f1);
  b.co();
  f1 = std::bind(fo2);
  b.setFo(f1);
  b.co();

  ///bind use _1
  //std::placeholders 代表的是绑定后的对象的参数, bind 的时候必须给的原函数的参数,
  // 按原函数的参数位置 e.g. f3 第一个参数是100， bind的对象的第二个参数给 f3
  // 的地二个参数， bind的第一个参数给f3的第三个参数
  auto bf3 = std::bind(f3, 100, std::placeholders::_2, std::placeholders::_1);
; bf3(1,2);

  FunO3 fo3;
  /// 当要把f3保存到class成员中时这种方法有问题,
  // 因为 bind 返回一个functionObjc, 保存的是f03的地址，当fo3销毁时，
  // 在调用bind的返回值就会发生错误 O MyGod!
  std::function<void(int)> f3 = std::bind(&FunO3::operator(), &fo3, std::placeholders::_1);

  /// 应该使用这个 上面的方法虽然可以调用但是在离开f3作用预后 调用一个
  // f3 的copy 可以去去不到bind 时fo3 中的成员对象， 可能是删除了 ???
  // 而这个就可以
//  std::function<void(int)> f3x = std::bind(fo3, std::placeholders::_1);
//  
  fo3(3);
  
  //如果在调用bind的返回值时 fo3 可能已经不存在可以传入obj 对象， bind
  //会copy fo3 来调用他
  std::function<void(int)> f3o = std::bind(&FunO3::operator(), fo3, std::placeholders::_1);

  ///EE bind 类静态函数
  std::function<void(int)> statF = std::bind(A::staticFunc, std::placeholders::_1);
  statF(30);

  return 0;
}
