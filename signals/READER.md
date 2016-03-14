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

+ 实际中信号是异步的，所以安全的除了形式是两段法，
  即在signal handle中只(异步通知)设置一个外表变量，(活在发送字节到管道)
  而在主逻辑中间隔的检查这个变量来察看是否收到了信号

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
7.  别用signal，用sigaction
        可以通过signal或者sigaction函数来设置信号处理函数，但signal函数太过古老，
        因此推荐使用sigaction。理由如下：
    1. sigaction可以提供更多接收到信号的信息。
    2. 调用完信号处理函数后重新设置处理函数不会对sigaction有影响，
       因为sigaction默认是不会去重置处理函数的，同时在执行处理函数会屏蔽掉该信号，也不会有竞争。
    3. signal函数在某些系统中会默认重启被中断的系统调用，而sigaction默认不会这样做。
    4. signal函数在多线程环境中的行为是未定义的，必须使用sigaction函数。
8.  正确处理被信号中断的系统调用
    1. 在没有传输数据前就被中断，这时系统调用返回-1, erron==EINTR
    2. 在传输数据途中被中断，这时不会返回错误，返回一个比期望小的值，
     

###可靠信号 不可靠信号
传统的信号都是不可靠的。（多个相同信号到达时时，只保留一个)
时事信号都是可靠的。（多个相同信号到达时时，都会安序保留)


### siganl
SIGPOLL SIGIO
SIGPOLL
用于异步IO 
SysV 用 SIGPOLL BSD 用 SIGIO 


###可重入(Reentrant)
```
1：什么是可重入：
当程序正在调用一个系统apiA, 还没返回，这是被一个signal中断，
程序跳动signal的处理函数在，但是该signal_handler_中又有对systemAPI A的调用，
这时称，重入A。即，前一个对A的调用还没返回，又有一个对A的调用。
+   信号处理要保持调用可重人函数


fork 后 子process继承父process的信号处理方式。 
子进程exec 后子进程重置被进程捕获的sig为默认动作


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


### 多线程和信号
？？？

### 同步化信号
kernel 2.6.22 后提供了一个*signalfd* 系统调用
来使用一个fd来接受信号，这样所有的信号接受操作都都是同步的，
这样就可以同步地处理信号，也不需要设置处理函数。
* 首先必须使用sigprocmask来屏蔽要使用signalfd来处理的信号
* 然后调用signalfd创建一个fd用来读取到达的信号。
  当被屏蔽的信号到达时，程序将不会被中断，
  也不会有处理函数被调用。信号会在fd中排队。
* signalfd创建的fd可以和其他fd一样：
   可以放在select, poll, epoll中；可以设置为非阻塞;
   可以为不同的信号创建不同的fd；


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
	sigaddset //对同一个signal多次调用没有问题
	sigdelset //对同一个signal多次调用没有问题

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
    在设置了block的信号后，程序可能想知道实际被block的信号有没有
    这是可以使用 sigpending 
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

### SA-INFO
使用 siginfo_t 


                  //signo 发送的信号 和 si->_si_signo 相同
void sigchld_handler(int signo, siginfo_t *si, void * p) 
{
    //ci_code 说明了信号产生的原因
    SI_* 对于所有信号都实用
    CLD_* 和 SIGCHLD 信号相关

	switch (si->si_code) {
		case SI_USER: //信号由kill raise 发送的
		case SI_TKILL:// 信号由tkill tgkill 发送
			DEBUG_LOG("SIGCHLD from pid=%d uid=%d, IGNORED",
					si->si_pid, si->si_uid);
			return; /* someone send use fake SIGCHLD */
		case CLD_KILLED: //SIGCHLD 信号引发 子进程被终结了
			DEBUG_LOG("child %d killed by signal %s",
					si->si_pid, signame[WTERMSIG(si->si_status)]);
			stop = 1;
			if (WTERMSIG(si->si_status) == SIGABRT)
				restart = 1;
			break;
		case CLD_TRAPPED: //
			DEBUG_LOG("child %d trapped", si->si_pid);
			return;
		case CLD_STOPPED: //child has stopped
			DEBUG_LOG("child %d stopped", si->si_pid);
			if(si->si_pid > 1) kill(si->si_pid, SIGCONT);
			return;
		case CLD_CONTINUED:
			DEBUG_LOG("child %d continued", si->si_pid);
			return;
		case CLD_DUMPED: //traced child has trapped
			DEBUG_LOG("child %d coredumped by signal %s",
					si->si_pid, signame[WTERMSIG(si->si_status)]);
			restart = 1;
			stop = 1;
			break;
	}
}


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

#### SIGCHLD
每当有进程终结时，系统就会给他的parent进程发送 SIGCHLD 信号
   此信号默认的机制是忽略,
注意child的终结和发送给parnet进程SIGCHLD的是异步的

waitpid 是SysV BSD 有自己的对应函数 wait3, wait4 
 3,4 的意思是他们的参数个数
 wait3(status, optins, NULL) == waitpid(-1, status, optins) for any child 
 wait4(pid, status, optins, NULL) == waitpid(pid, status, optins) for specific child

## SIGXCPU
  当程序的cpu使用时间大于 ulimit cpu time 的soft限制时，会给程序每秒
     发送一个SIGXCPU,默认行为的终止，程序可以捕获，当到到hard 限制时
     直接发送SIGKILL

