# 个人学习linux process pthread的笔记

并发，多处理器编程概念

不可能性： 即一种并行算法是否可以实现
 
互斥特性
-----------------------------------
  如果一个协议，需要保证互斥性，那他可以从这几个方面来考察
  无死锁
  无饥饿
  等待时间

+ 互斥需要的是持续通信
  
几种典型的并发问题
1. 消费者-生产者： 挂起，唤醒，同步
2. 读者-写者，多读，1写, 和1的差异， 数据一致存在，读者不会消费数据
             数据不会消失，读写操作需要互斥。
3. 哲学家进餐： 是资源竞争，处理dadcolk 问题


并发系统中存在两种通信形式
1. 瞬时通信,要求通信双方同一时间都参与通信
2. 持续通信: 允许通信双方在不同时间参与通信



*   WhyUsePthread.md "线程的好处和使用范围"
*   PTHREAD.md pthread 的学习笔记


#signal
Doc https://www.ibm.com/developerworks/cn/linux/l-cn-signalsec/

Qus. 多线程共享信号的什么？
      1.
    哪些是线程自有的？
      2.
    给一个多线程发送sigal时，谁会收到这个signal?
      随即一个线程,并且没有屏蔽此signal的

1. 所有线程共享 signal的handle 函数，
2. 每个线程有自己的sigmask, 可以调用 pthread_sigmask 来设置
3. 当调用pthread_create 创建的新线程会*继承*夫线程的sigmask

    多线程相关的signal函数
      pthread_sigmask
      pthread_kill
      sigwait
      sigwaitinfo

3. 以同步的方式处理异步信号
    用一个专门的线程来等待要处理的信号, 而其线程则不被打断
     多线程程序同步处理信号模型
     1. 主线程设置希望处理的sigmaks， 并屏蔽他们, 使得所有的其他线程都屏蔽这些signal
     2. 这种这些sigmask的handle
     3. 创建一个线程专门来处理signal //使用sigwait/sigwaitinfo 而不是signal, sigaction
     4. 创建其他工作线程

     see sigwait_t.c 3

注意事项

在基于 Linux 的多线程应用中，对于因为程序逻辑需要而产生的信号，
可考虑使用同步模型进行处理；而对会导致程序运行终止的信号如 SIGSEGV 等，
必须按照传统的异步方式使用 signal（）、 sigaction（）注册信号处理函数进行处理。
这两种信号处理模型可根据所处理的信号的不同同时存在一个 Linux 应用中：

    不要在线程的信号掩码中阻塞不能被忽略处理的两个信号 SIGSTOP 和 SIGKILL。
    不要在线程的信号掩码中阻塞 SIGFPE、SIGILL、SIGSEGV、SIGBUS。
    确保 sigwait() 等待的信号集已经被进程中所有的线程阻塞。
    在主线程或其它工作线程产生信号时，必须调用 kill() 将信号发给整个进程，而不能使用 pthread_kill() 发送某个特定的工作线程，否则信号处理线程无法接收到此信号。
    因为 sigwait（）使用了串行的方式处理信号的到来，为避免信号的处理存在滞后，或是非实时信号被丢失的情况，处理每个信号的代码应尽量简洁、快速，避免调用会产生阻塞的库函数。

