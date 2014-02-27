 vim: ft=markdown   


最佳实践
---------------------------------
不要对新产生的线程的执行顺序有任何的假设

内存模型
--------------------------------------
每个线程对于其他线程都有自己的视角，
不同的线程所看到的世界不一定相同。

如多个线程都读一个内存，线程都内存是
原则的，但是有可能每次只读8bit， 而变量的
大小是32bit， 这就造成的不是原则的读取，
比如读取非对齐的内存，要多次读取

线程的私有数据
-----------------------------------
* 线程ID
* 执行上下文
* 栈
* nice值
* 信号屏蔽字
* errno变量
* 线程私有数据


线程共享数据
-----------------------------------
在进程中，所有线程共享
* 可执行的程序文本
* 对内存 variabe pthread_cond...
* 打开的文件描述符


POSIX 线程 API
--------------------------------------
* 所有的POSIX api都以pthread 开头
* pthread 的错误处理，不通过设置errno，来表示错误，
     对用pthread函数，如果返回非0，就是出错，
     返回值就是错误吗，使用strerrno来显示

Qus.
1. 主线程如何等待所有的子线程，而不是某一个(pthread-join) ??
     join 每个线程

2. 如果一个线程crash了会影响别的线程吗？
     all process will crashed

3. 同一个process生成的线程，调用getpid 相同吗? 
        相同。

4. 主线程getpid 返回的值和自己的pthread id相同吗？
       不相同。pid
       gettid()  returns  the  caller's  thread  ID (TID).  
       In a single-threaded process, the thread ID is equal to the 
       process ID (PID, as returned by getpid(2)).  In a multi‐threaded process, 
       all threads have the same PID, but each one has a unique TID.

5. gettid 和 pthread-self 返回的值相同吗?
       不相同， gettid 返回的值时linux kernel 对一个线程的标记
       而pthread-self 时 pthread 线程库对一个线程的标记

6.  glibc 不提供gettid 要使用syscall 调用




EE
////线程不会共享局部变量,因为这些局部变量都是在调用
//堆上的，每个线程有自己的调用堆栈
所以不需要使用锁

EE 首先需要安装lib
compile and link command
`cc -D_REENTRANT file -pthread`

EE wait 的宏解析
W 表示wait
WNOHANG: w-no-hang //return immedatle if no child has exited


wait he waitpid (status )返回状态解析宏
WIFEXITED w-if-exited
正常终止 返回true，可用WEXITSTATUS取出推出参数

WIFSIGNALED w-if-signaled
异常终止返回真, 可用WTERMSIG取出使进程异常终止的

WIFSTOPPED w-if-stopped
WIFCONTINUED w-if-contionued

pthread
NTPL, 使用top查看新建的线程时PID显示的各自不同，但是用getpid，得到的pid数是
一样的，为啥？？？


EE 共享内容
  1. 进程地址空间
  2. 文件描述符

**主线程的特殊性**
1. 线程的参数不同。 主线程使用crt0.o文件初始化.
2. 主线程结束时，不会等待其他线程。也就是说只用主线程终结，其他线程也会被终结
3. 如何不想这样，可以使用`pthread_exit`如不是从main 中 return
4. 在大多数系统上主线程运行在默认对在

EE **线程蒸发**

**线程分离**（回收）
 `pthread_detach` 分离一个线程
  线程是可结合的(joinable) 或者是分离的(detavhed)
  一个可结合的线程能够被其他线程，杀死和回收其资源，
  一个以分离的线程不能被其他线程杀死或回收 ???，在他死亡后会自动回收资源
在一个线程结束时，如果没有被分离，就会保留其虚拟内存，stack和其他系统资源。
分离线程意味着通知系统不再需要此线程。系统将回收分配给他的资源。 和父进程等待子进程类似.


EE 线程类`pthread_t`型

EE `pthread_create/4`  建立一个线程
      第一个参数不能为NULL

EE `pthread_exit` 终止调用的线程
EE `pthread_self` 得到自己的线程ID,所以的线程操作都需要线程ID

EE `pthread_equal` 判断两个线程是否相同


EE `pthread_join/2` 用来活动指定线程的退出码
  前提： 这个线程必须是joinable 的
  调用者会被阻塞直到指定线程终止。指定的线程会自动分离
  当join返回时，指定的函数已经被分离。 类似于waitpid
  不能join 已经detach 的线程会返回错误

  Qus. 自己能join自己吗？  不可以，会调用失败

EE 线程的创建
   1.主线程随进程的创建而创建。
   2.其他线程可以通过`pthread_create`创建
   3.接受到`SIGEV_THREAD`时也会创建一个线程

EE 线程创建后就会在就绪态。**进程到用的无序性** _从pthread-create返回与调用新的线程之间不存在同步关系_
EE 线程终止
  1. 线程通常从启动函数返中返回来终止自己
  2.`ptherad_exit`
  3.`ptherad_cancel` 调用`ptherad_canceld`返回码总是`PTHREAD_CANCELLED`
   调用的cancel 并不意味着thread就可以终止，thread必须运行到一个cancel point才能终止
   如果没有，那thread不会终止

  如果线程已经分离则会立即进入回收状态,否则就会出现僵尸线程.(注意线程和进程的相似性,linux下线程其实就是进程)

void pthread_cleanup_push 用来在ppush 和pop 直接执行时遇到在自己被cancel/ exit 
时执行push 中的handle
void pthread_cleanup_pop(execute)
 execute参数表示执行到pthread_cleanup_pop()时是否在弹出清理函数的同时执行该函数，
 为0表示不执行， 非0为执行；这个参数并不影响异常终止时清理函数的执行。
 这两个函数必须成对出现

EE 系统资源（malloc,mmap) _可以在任何时候由任何线程释放_. 
   信号量，互斥量,条件变量,信号灯可以由任何线程销毁. 前提没被解锁或现成等待. _但是有只有互斥量的主人才能释放解锁,所以线程终止时一定要解锁_


EE pthread_once(once-vaulable, once-routine)
   用于希望只被执行1次的代码段
   参数是一个标记变量 和一段执行过程

Mutex
-----------------------------------
* 加锁 `pthread_mutex_lock(&mutex)`
    陷阱： 1. 同一线程对同一Mutex加锁两次，会造成死锁, 因为第二次加锁时
              线程自身也会阻塞
* 避免 AB BA

读写锁 read/write lock
-----------------------------------
* 一个读写锁有三种状态， 不加锁，读锁，写锁
* 同一时刻只允许一个线程持有写锁, 如果对一个处于写锁状态的RWL加锁，则线程会阻塞
* 同一时刻允许多个线程持有读锁,   如果对一个处于读锁状态的RWL加读锁，可以加锁，
                                                             加写锁，线程会阻塞
* `pthread_rwlock_init/2`
* `pthread_rwlock_destroy/2`
* 只读方式加锁 `pthread_rwlock_rdlock/1`
        常常用于find search 共享数据
        实现都限制了对大的读锁数量，需要检查返回值
* 读写方式加锁 `pthread_rwlock_wrlock/1`
* 解锁       `pthread_rwlock_unlock/1`

condition 条件变量
---------------------------------------------
*  conditon 是用来干啥的? 
     使用条件变量可以使得,一个线程在等待某个特定条件时不需要忙等待.
     如果等待的条件不满足，则线程会休眠，直到其他线程唤醒他
     条件变量提供了使多个线程以无竞争的方式等待特定条件发生

* 条件变量本身需要由meutx保护，进程在改变条件变量前必须先lock mutex
* cond 只是一个标记代表对应的谓词,但并不是谓词

* `pthread_cond_t`
* `pthread_cond_init/2 | PTHREAD_COND_INITIALIZER`
* `pthread_cond_destory/1`

* `pthread_cond_wait/2(cond, mutex)`
      条件不满足时，主动睡眠, 调用pthread_cond_wait/2
      线程必须在lock mutex后才能调用 pthread_cond_wait/2,
      pthread_cond_wait 会把调用线程放入等待线程队列，然后解锁muxt(这两步是原子的)
      当等待的线程被唤醒后(从pthread_cond_wait/2 返回时)，mutex,会被再次锁住
      
* `pthread_cond_signal/1` 
     唤醒一个在指定cond上的wait的线程， 向等待线程发送信号时不需要持有mutex
* `pthread_cond_brocast/1`
     唤醒所有在cond上wait的现场。
        如果这些现场都要争抢统一资源，则不建议使用brocast, 群惊的开销
        比较大。使用signal更好一些
        如果是多个生产者一个消费者，则消费者必须使用brocast 否则使有些worker一直wait

线程属性
---------------------------------------------------
* `pthread_attr_t`

* `pthread_attr_init/1`
* `pthread_attr_destroy/1`

* `pthread_attr_getdetachstate/2` 得到一个pthread_attr_t 的分离属性
* `pthread_attr_setdetachstate/2` 设置一个pthread_attr_t 的分离属性
 <script=c>
     //e.g. 让一个线程一分离状态启动
    pthread_attr_t pattr;
    pthread_attr_init(&pattr);
        pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_DETACHED);
        pthread_create(&tid, &pattr, call, NULL);
    pthread_attr_destroy(&pattr);
 </script>


Mutex 属性
------------------------------------------------------
* `pthread_mutexattr_t`

* `pthread_mutexattr_init/1`
* `pthread_mutexattr_destroy/1`

* mutex 类型 不同的锁类型在试图对一个已经被锁定的互斥锁加锁时表现不同
   * `PTHERAD_MUTEX_NORMAL 类型, (默认的)  不做任何的错误检查和死锁检测
             通常和 PTHREAD_MUTEX_TIMED_NP` 相同
   * `PTHREAD_MUTEX_TIMED_NP`
            这是缺省值，也就是普通锁。当一个线程加锁以后，
            其余请求锁的线程将形成一个等待队列，并在解锁后按优先级获得锁。
            这种锁策略保证了资源分配的公平性。
   * `PTHERAD_MUTEX_ERRORCHECK  PTHREAD_MUTEX_ERRORCHECK_NP`
           检错锁，如果同一个线程请求同一个锁，则返回EDEADLK，
           否则与PTHREAD_MUTEX_TIMED_NP类型动作相同。这样就保证当不允许多次加锁时不会出现最简单情况下的死锁。
   * `PTHERAD_MUTEX_RECURSIVE | PTHREAD_MUTEX_RECURSIVE_NP`
           可重入的muext， 就是一个线程可多次加锁同一个mutex，而不会死锁
           嵌套锁，允许同一个线程对同一个锁成功获得多次，并通过多次unlock解锁。
           如果是不同线程请求，则在加锁线程解锁时重新竞争。
   * `PTHREAD_MUTEX_ADAPTIVE_NP` 适应锁，动作最简单的锁类型，仅等待解锁后重新竞争。

* `pthread_mutexattr_gettype/2`
* `pthread_mutexattr_settype/2`


* 线程专有数据， 每个线程都有自己的一个
` 变量类型 pthread_key_t
  使用 pthread_key_create/2 来创建该变量

  如果使用 pthread_key_t 要设置线程专有数据通过
    pthread_key_setspecific/2
    pthread_key_getspecific/2
    来，不能直接设置

    pthread_key_delete
  
  GNU _thread
