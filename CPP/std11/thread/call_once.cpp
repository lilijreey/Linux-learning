/**
 * @file     call_once.cpp <mutex>
 *           std::call_once(std::onc_flag& flag, func, args...)
 *           只执行指定func 一次，即使多个线程调用
 *           和 pthread_once 类似
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     05/10/2013 01:44:48 PM
 *
 */

#include <iostream>
#include <thread>
#include <mutex>

 
std::once_flag flag; //同一个flag ,保证一次
void do_once()
{
    std::call_once(flag, [](){ std::cout << "Called once" << std::endl; });
}

/// 也可使用 在一个函数中定义static 变量来达到只初始化一次的效果.
// 在C11中不同的线程,会等到static变量初始化完成后在执行，不会出现race conditons
//T getInstacne(){
// static xxx;
// return xxx;
//}
 
int main()
{
    std::thread t1(do_once);
    std::thread t2(do_once);
    std::thread t3(do_once);
    std::thread t4(do_once);
 
    t1.join();
    t2.join();
    t3.join();
    t4.join();

}

