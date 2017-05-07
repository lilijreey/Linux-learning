/** vim: foldmethod=marker
 * @file     mutex.cpp
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     05/09/2013 11:48:11 AM
 *
 */

#include <thread>
#include <list>
#include <stdio.h>
#include "threadGuard.hpp"

/// std::mutex
//  member-funcs
//  * `lock/0'
//  * `trylock/0'
//  * `unlock/0'
//   不用自己直接调用lock,unlock, 可以使用std::lock_guard 

/// std::lock_guard/1-2 template
//   一个 mutex wrapper 提供 RAII-style 在一个scoped block.
//   当一个lock_guard 构造是他试图lock一个mutex，在离开lock_guard
//   scope destruction lock_guard 时会unlock mutex
//   
//   * std::lock_guard<std::mutex> `guard/2'(m, std::adopt_lock);
//       第二个参数是加锁策略
/// std::adopt_lock 
//       std::adopt_lock 是个站位参数，说明lock_guard 构造时不要加锁，
/// std::defer_lock 
//       延迟加锁， 在构造是不加锁
/// std::try_to_lock 
//         用try_to_lock 加锁
//         
/// constexpr defer_lock_t	defer_lock { };
/// constexpr try_to_lock_t	try_to_lock { };
/// constexpr adopt_lock_t	adopt_lock { };
//  
//          前提： 当前线程必须已经lock mutex， 在unlock
//          mutex时，mutex必须存在，
//          就是 mutex必须比lock_guard 活的长
//       e.g. std::lock(m1, m2);
//            std::lock_guard<std::mutex> guard(m1, std::adopt_lock);
//            std::lock_guard<std::mutex> guard(m2, std::adopt_lock);
//            


/// std::unique_lock/1-2
//  unique_lock 只有move语义没有copy语义
//   一个可以延迟lock和move ownership， 比lock_guard 更灵活
//   lock/0 会抛出异常
//   trylock/0
//   unlock/0


// 两个线程并行insert 一个list，使用mutex保护
// 数据是全局的并不好， see II 改为面向对象的保护
#if 0 //{{{
std::list<int> list;
std::mutex mutex;

void insert(int i)
{ //如果没有加锁， 插入将是不可预知的
  std::lock_guard<std::mutex> guard(mutex);
  list.push_back(i);
}

void run(int num)
{
  for (int i=0; i< 10000; ++i)
    insert(num);
}

int main()
{
  std::thread t1(run, 1);
  std::thread t2(run, 2);

  { //same as t1.join() t2.join();
    ThreadGuard gt1(t1);
    ThreadGuard gt2(t2);
  }
  printf("t1 t2 run over\n");
  printf("%lu\n", list.size());

  //  for (auto i : list)
  //    printf("%d ", i);

  return 0;
}
#endif //}}}

class Data
{
  //nocopyable
public:
  void insert(int i)
  {
    std::lock_guard<std::mutex> guard(_mutex);
    _list.push_back(i);
  }

  void show() const
  {
    printf("size:%lu\n", _list.size());
    for (auto i : _list)
      printf("%d ", i);
  }
 private:
  std::mutex _mutex;
  std::list<int> _list;
};

void run(int num, Data &data)
{
  for (int i=0; i< 10000; ++i)
    data.insert(num);
}

int main()
{
  Data data;
  std::thread t1(run, 1, std::ref(data));
  std::thread t2(run, 2, std::ref(data));

  { //same as t1.join() t2.join();
    ThreadGuard gt1(t1);
    ThreadGuard gt2(t2);
  }
  data.show();

  return 0;
}
