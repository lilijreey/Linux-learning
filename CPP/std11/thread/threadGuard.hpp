/**
 * @file     ThreadGuard.hpp
 *          使用RAII 来join
 *           
 *
 * @author   lili <lilijreey@gmail.com>
 * @date     05/08/2013 01:23:09 PM
 *
 */

#ifndef  THREADGUARD_INC
#define  THREADGUARD_INC

#include <thread>

class ThreadGuard
{
 public:
  explicit ThreadGuard(std::thread &t)
      : _t(t)
  {
  }


  ~ThreadGuard()
  {
    if (_t.joinable()) 
      _t.join();
  }

  ThreadGuard(const ThreadGuard &) = delete;
  ThreadGuard& operator=(const ThreadGuard &) = delete;
  
 private:
  std::thread& _t;
};

// example
// void f()
// {
//   int local_var=3; //线程使用了局部变量的引用,
//   std::thread t(func(local_var));
//   ThreadGuard tg(t); //保证在t运行结束前不会推出f
// }
// 
#endif   /* ----- #ifndef threadguard_INC  ----- */
