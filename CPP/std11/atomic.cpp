/**
 * @file     atomic.cpp
 *           c11 原子操作库.
 *               
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     05/07/2013 06:04:54 PM
 *
 */

#include <atomic>
#include <stdio.h>

/// 原子类型 std::atomic 是个模版.
//GCC 使用__sync_lock* 实现;
// 特化的atomic 类型 std::atomic_char
//std::atomic_char 	std::atomic<char>
//std::atomic_schar 	std::atomic<signed char>
//std::atomic_uchar 	std::atomic<unsigned char>
//std::atomic_short 	std::atomic<short>
//std::atomic_ushort 	std::atomic<unsigned short>
//std::atomic_int 	std::atomic<int>
//std::atomic_uint 	std::atomic<unsigned int>
//std::atomic_long 	std::atomic<long>
//std::atomic_ulong 	std::atomic<unsigned long>
//std::atomic_llong 	std::atomic<long long>
//std::atomic_ullong 	std::atomic<unsigned long long>
//std::atomic_char16_t 	std::atomic<char16_t>
//std::atomic_char32_t 	std::atomic<char32_t>
//std::atomic_wchar_t 	std::atomic<wchar_t>
// ...

///内存模型 GCC4.8 才实现
//

///std::atomic_flag
// 必须使用 ATOMIC_FLAG_INIT 初始化
/// clear/1
// 原子的用来清除, 设为false
/// test_and_set/1 -> bool()
// 原子的设置为ture 返回原来的值
// Qus. atomic_flag 能干什么， 为什么要出现atomic_flag ?


int main()
{
  std::atomic_int ai(0);
  //std::atomic_int ai = 0

  //std::atomic member funcs
  ///is_lock_free/0 -> bool() 检测所有的原子操作是不是不加锁的。
  //实现中原子类型可以是使用加锁实现，也可以是使用cpu提供的原子操作实现的
  if (ai.is_lock_free())
    printf("atomic is lock free type\n");
  else
    printf("!!! atomic is not lock free type\n");

  ///load/0-1 原子get
  //内存屏蔽参数  memory_order 
  //std::memory_order_relaxed, 
  //std::memory_order_consume, 
  //std::memory_order_acquire 
  //std::memory_order_seq_cst. //默认参数
  printf("init:%d\n", ai.load());
  for (int i=0; i<100; ++i)
    ++ai;
  printf("init:%d\n", ai.load());

  /// store/1-2原子set
  //内存屏蔽参数  memory_order 
  //std::memory_order_relaxed //不执行内存屏蔽
  //std::memory_order_release //不执行内存屏蔽
  //std::memory_order_seq_cst //默认参数，执行内存屏蔽
  ai.store(20);

  //EE oldT `exchange/2' 和store/2 类似，返回原先的值
  //CAS
  printf("%d\n", ai.exchange(21));

  return 0;
}
