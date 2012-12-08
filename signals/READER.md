in /usr/include/bit/signum.h
默认动作：(see) man(7) signal man(1) kill

信号是异步事件，只能告诉内核，在此信号出现是的动作，而不能简单的检测一个变量， 来判断是否出现一个信号。

##信号操作
+   接受信号 +   无视信号
+   屏蔽信号

###信号处理的3种方式：
1.忽略: 有两种信号不能忽略（SIGKILL，SIGSTOP）
2.捕捉: 通知内核在出现要捕捉的信号时调用一个用户函数。
有两种信号不能被捕捉（SIGKILL，SIGSTOP）
3.执行系统默认动作：

### 设置一个信号的处理方式后对该信号的处理一直有效

### Core file 
+   see core(5) man page
+   生成 Core file
    +   一般不生成core文件 可以使用 ulimit -c 50 // core file size 
        or ulimit -c unlimited 开启core文件生成
+   core file name
    +   /proc/sys/kernel/core-pattern 控制core 文件的名字

### 不会产生corefile的情况
+   没有写权限
+   写的目录不存在
+   process资源 RLIMIT——CORE 0
+   ....

### 实践准则
0.  使用sigaction siginfo
1.  屏蔽正在处理的signal，使用siginfo默认屏蔽当前处理的singal
2.  信号处理函数保证可重入
3.  不要使用io函数
4.  errno的处理
5.  SIGABRT 采用默认行为，或者在hanler中exit assert可以终结process
6.  SIGTERM 采用自己的handler，优雅的终结process。通常是kill


###可重入(Reentrant)
1：什么是可重入：
当程序正在调用一个系统apiA, 还没返回，这是被一个signal中断，
程序跳动signal的处理函数在，但是该signal_handler_中又有对systemAPI A的调用，
这时称，重入A。即，前一个对A的调用还没返回，又有一个对A的调用。
+   信号处理要保持调用可重人函数


fork 后 子process继承父process的信号处理方式。 
这个可能是exec后子进程被父进程捕获的sig都将回复默认值


可以通过发送信号0来检测一个进程是否存在： 如果不存在，kill返回错误errno位ESRCH
//signal 可以打断sleep，


对于SIGTERM 该信号只会有kill发出，可以捕捉该信号，但是，返回后进程自动终止

// 主进程给自己发singal，子进程会收到吗。
// 不会
// parent process send signal to self,  child process cant resevce this signal

对用SIGARLM 来说不对，只有设置一次就行
对用使用signal 设置的handle，只能处理一次，(自动复原乘def）
第二次收到相同信号时，就不会执行原来的函数了。

SIGINT 按下<C-c> 时发出 常见的捕捉信号，用来处理用户产生的终端符
SIGTERM 常见的捕捉信号，捕捉以便在结束前执行必要的清理工作, 发送SIGTERM并不会导致发送KILL

SIGHUP 当一个前台进程的tty关闭时，会收到SIGHUP， 后台process不会收到
对于daemon通常监听此信号，来重新读取配置文件，因为这个信号不会是tty发送的

SIGABRT 有abort() 函数生成， 主要用来产生core file

### 0号sig
通常检查pid是否被使用 不会发送但是会检查

给自己发信号 raise(int signo) 
给进程组的所有进程发信号
	killpg(pgrp, sig)

### 信号的继承
???

### 一个信号处理return后会发生什么
通常一个syscall 会返回一个 EINTR 的错误
要重新启动这个syscall 一般会这样写
int cnt;
while((cnt = syscall) == -1 && errno == EINTR)
    continue;
if (cnt == -1)
    exit

### NSIG 最大的信号number
64 包括实时信号 define in signal.h file

### 信号集(block set)
对象是signals mask 本身
+   信号集的初始化,信号集必须初始化但是不能用memset
	sigemptyset : 初始化为空机会
    sigfillset  : 初始化为包含所有的signal(包括 KILL,STOP)
+   信号集的增减
	sigaddset
	sigdelset

+   查询指定信号
	sigismember

###   sigprocmask
设置process signal mask, 对象是process
    +   SIG_BLOCK_ 把指定的集合与当前block的集合相交产生
    不允许阻塞SIGKILL，SIGSTOP，sigprocmask 会忽略对这两个signal的阻塞要求 
    的集合为新的block集合
    +   SIG_UNBLOCK_ 把当前block集合中的指定remove。如果block中没有某一signal，不会导致错误
    +   SIG_SETMASK_ 重置signal mask

### pending signals
    当kernel 产生一个block的signal，把这个signal添加到
    process‘s pending signal 集合,当解除block时会发送这些信号
+   sigpending() 获得前的pending signal 集合 sigpending(out)
+   pending 的信号不是队列，多个相同xignal之保存一个

### sigsuspend

### signal mask
+   对每一个process kernel都管理着一个signal mask。一个屏蔽的信号集.
+   阻塞一个signal后，在阻塞期间，如果有多个该阻塞信号产生，kernel只会保留一个，在阻塞该signal后发送,不保存相同的信号多个
+   实际上signal mask是一个thread属性，可以使用pthrad_sigmask()_

#### structs
+   sigaction `{
	       void	(*sa_handler)(int); // signal handler SIG_DFL,SIG_IGN.
	      void	(*sa_sigaction)(int, siginfo_t *, void *);
	       sigset_t   sa_mask; //block mask
           //标记 默认使用sa_handler作为信号handler
           // 设置为 SA_SIGINFO 使用 sa_sigaction 
	       int	  sa_flags; 
	       void	(*sa_restorer)(void); // 过时了的
        }
`
+   初始化一个sigaction
    sigemptyset(&act.sa-mask)
    act.sa_sigaction = handler;
    act.sa-flags= SA_SIGINFO
    sigaction(signal, &act, NULL)

+   siginfo_t_
    对所有signal si_signo
                 si_errno
                 si_code  说明信号发生的原因
                 都有效。 其他member只对特定singal有效
+   sigset

### sigaction
提供更精确的控制能力，使用sa-sigaction 作为处理函数，需要指定sa-flags=SA-SIGINFO
正在被处理的signal默认在sa-mash 中.阻塞当前处理的signal。 
不阻塞 设定为SA—NODEFER
+   sa-flags 控制信号的处理行为
+   一个sigaction 可为多个signal 设置handler

### sa-flags
SA-RESTART 如果一个syscall是因为signal中断则signal处理完后重启syscall
    per-signal set. 对I/O syscall wait*, 
    对于poll,ppoll select pselect, epoll_wait epoll_await, sleep 
    等函数没用。这些函数永远不会自动重启
    more see signal(7)


SA-INFO 使用actinfo
SA-ONSTACK 使用专门的stack处理handle
SA-NODEFER 不屏蔽正在处理的signal

### signal handler 中errno的使用
因为errno是个全局变量所以在handler中使用时要先记录errno的原始值
e.g. int saveErrno = errno; /do... errno = saveErrno

### sigaltstack 
专门用于signalhanler 的stack
sigaction 的sa-flags 要设置为SA-ONSTACK
???
### siginterrupt(sig, flag)
设置被sig中断后的行为，0重启，1返回EINTR

### sig-atomic-t
C99 保证原子的操作的一个整数类型
最好用volatile 声明

#### sigqueue(pid-t pid, int signo, const union sigval value)
和kill类似也是发信号给进程但是可以发送附加信息
接受信号的进程如果使用siginfo 那si-int 对应value si-code被设为SI-QUEUE

### More See
provide background on the implementation of signals in Linux.
