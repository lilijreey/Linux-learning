协程 co-operative routines

用户态线程,需要手动让出 yield


起源
Simula/Modula2 Languages 1963
适合实现迭代器,合作式多任务,管道,状态机


实现方法 mechanism


不同调用机制实现协成的方法
##C
 如果将每个协程的上下文（比如程序计数器）
 保存在其它地方而不是堆栈上，协程之间相互调用时，
 被调用的协程只要从堆栈以外的地方恢复上次出让点之前的上下文即可，
 这有点类似于 CPU 的上下文切换，



本质, 保持当前副作用(环境)
   包括调用堆栈,CPU-regisers
   执行coroutins 是必须给每个coro 指定一个存储他contex 的变量

1. 使用setjmp/longjmp
2. sigsetjmp/siglongjmp



* 如何实现自己的CTB/context
  如何隔离不同corouitie


优缺点



开源库
* POSIX ucontext
       #include <ucontext.h>
       makecontext
       int getcontext(ucontext_t *ucp);
       int setcontext(const ucontext_t *ucp);
       swapcontext
     使用汇编实现
 
      
