/**
 * @file     thread.cpp
 *        C11 线程类学习笔记, 编译时必须使用 -lpthread 否则会崩溃
 *          大部分的并行特性在GCC4.8后才实现.
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     05/07/2013 10:58:30 PM
 *
 */
#include <stdio.h>
#include <iostream>
#include <thread>
#include <functional>

/// static-func
// *`hardware_concurrency/0' -> unsigned
//  返回最大的并行线程支持, 如果不可计算或没有定义返回0 
 
/// member-type 
/// @native_handle_type thread的实现类型（平台相关类型） pthread_t

/// member-func
// * `native_handle/0' 返回平台相关的thread句柄， 比如使用posix
//     实现的thread native_handle/0 返回 pthread_t
//     
// * `join/0' 等待指定线程结束.
//       前提线程必须是joinable 的.
//       join会cleans up所有的线程资源。当一个join返回时， 
//       这个线程对象已经和原来的线程没有任何关联了
//       Qus. 如果多个线程join同一个thread会怎样？
//       
// * `joinable/0' -> bool 检查是否是可join的
//       当线程没有运行时是 不可joinable的 
// 
// * `get_id/0' -> thread::id classs
//        没有执行的和已经结束的 thread class instance get_id/0 返回的值相同
//        如果thrad 还没有运行在返回一个默认的 std::thread::id 类型
//        得到当前线程的id `std::this_thread::get_id/0'
//
// * `detach/0' -> void() 使线程和thread class 分类
//               当线程结束时，会自动回收所有资源
//               在调用detach/0 后
//               thread class 和任何线程都没有关系了，
//               thead.joinable() == false
//               thread.get_id() == std::thread::id()
//          一般使用detach/0 来上线程在后台运行，
//          前提， thrad.joinable 必须是true时才能detach
//


/// member-class
//  @id 公共成员类，用来唯一标记thread 

/// std::this_thread::sleep_for

/// 常用技术
// * 在不同线程间传递数据
// * move transferring ownership of a Thread 
//    thread 支持move语义，可以在不同thread 对象中转换对应的线程
//    std::thread t1(func1);
//    std::thread t2(std::move(t1)); 现在t2 是func1 线程 
//    t1 = std::thread(func2);
//    t2 = std::move(t1) // move 时t2已经对应func1 了， 
//        赋值后， t2 现在对应func2, 原来的func1被执行 detach 了
//    thread支持move 后可以线程返回 线程的函数
//      e.g. 
//       std::thread f()
//       {
//           void func1();
//           return std::thread(func1);
//       }
//    
//   
 
void run()
{
  printf("hello\n");
}

void run1(int i)
{
  printf("i is:%d\n", i);
}

void updata(int &i)
{
  i = 3;
}

int main()
{
  std::thread t(run); //run 一个线程

  std::function<void()> br = std::bind(run1, 3);
  std::thread t1(br); //使用bind 还是，传递function 给 thrad

  t.join();
  t1.join();

  //测试 线程joinable
  std::thread nt;
  printf("nt joinable:%d\n", nt.joinable());
  printf("t1 joinable:%d\n", t1.joinable());

  /// passing arguments to thrad
  //线程的默认参数都是值传递的，如果需要引用用使用std::ref,std::cref
  //需要一个int的引用但是 thread 并不会
  //thrad 的参数机制和std::bind 相同
  int i=4;
  std::thread t2(updata, std::ref(i)); //传递引用参数时必须使用std::ref
  t2.join();
  printf("updata:%d\n", i);

  if (t.get_id() == t2.get_id()) //不执行的thrad get_id 返回默认的id类型
    printf("execution finished get_id return default id object\n"); //is true
  std::thread t3;
  if (t.get_id() == std::thread::id())
    printf("true\n"); //is true
  //打印non-execution
//  std::cout << "tid:" << t.get_id() << " t2.id:" << t2.get_id() << std::endl ;
  
  return 0;
}
