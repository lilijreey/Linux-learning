Daemon

Qus. 
1.  没有控制终端的一定就是守护进程吗?

###守护进程的作用
1.  long-lived. Often, a daemon is created at system startup
    and runs until the system is shut down.
2.  It runs in the backgroud and has no controling terminal.
    是为了确保，kernel永远不会自动生成任何作用控制，或终端
    有关的信号(SIGINT,SIGTSTP,SIGHUP)
    SIGHUP 通常来用来重启server

###特点:
+   必须是init的子进程，没有控制器终端， TTY=? TGPID = -1,
+   大部分系统守护进程是在系统自举时启动的
      PID  PPID  PGRP TPGID  SESS TT       COMMAND
        1     0     1    -1     1 ?        /sbin/init
        2     0     0    -1     0 ?        [kthreadd]
        3     2     0    -1     0 ?        [ksoftirqd/0]
        6     2     0    -1     0 ?        [migration/0]
+   用户层守护进程都是 
    1.  会话的首进程 (SESS 和PID相同)
    2.  进程组的组长进程 (PGRP 和 PID相同)
    3.  会话和进程组中唯一的进程
      PID  PPID  PGRP TPGID  SESS TT       COMMAND
     1668     1  1668    -1  1668 ?        //bin/dbus-daemon --fork --print-pid 5 --print-address 7 --session
     1625  1529  1625    -1  1625 ?        gnome-session --session=ubuntu
可以发现,User daemon SESS=PGRP=PID,or SESS=PGRP=PID+1 和PPID没关系

创建函数 
daemon  0 success 

###编程规则,
目的:防止产生不需要的交互 让SESS=PGRP=PID，tty=? 
1.  umask 设置为0(没有限制) 以防止从父进程那继承来的mask有限制
    目前为止，PID肯定不等于SESS, tty=!?
2.  fork, 父进程exit. 
    +   如果是从shell执行的，这样可以使得shell认为命令已经执行完成,shell可以返回 
    +   确保子进程不知一个组长进程，为创建新的会话做准备
    目前为止，PID肯定不等于SESS,PGRP, tty=!?
3.  setsid() 创建一个新会话, V系统要求再次fork, 父进程exit
    目前为止，PID=SESS=PGRP, tty==?
4.  更改继承来的工作目录到根目录。有可能当前的dir是在一个挂载的文件系统上。
    这就导致这个挂载的文件卸载不掉
5.  close所有打开的文件fd,把0,1,2,重定向到/dev/null. 
    +   daemon 调用一个使用I/O的库时，不会出错这样会隐藏所以的输出

###出错记录
    由于Daemon没有控制终端，所以不能把信息输出到标准输出上.
    一般使用syslog系统来输出信息

###单例进程 任意时刻只有一个运行的程序副本
    使用文件锁实现，传统上一个daemon会在 /var/run/下面建立一个 name.pid 
    的文件，把自己的pid写入，然后加锁。每次运行daemon都有检测该文件是否被枷锁
###Daemon 惯例
+    配置选项，通常放在/etc name.conf
+    通常捕捉SIGHUP信号，来重新都去配置
+   命名通常以d结尾， inetd, httpd, 
+    支持暂停，重启,关闭，打开

SIGHUP 当一个前台进程的tty关闭时，会收到SIGHUP， 后台process不会收到
对于daemon通常监听此信号，来重新读取配置文件，因为这个信号不会是tty发送的

###daemon 的启动
现在对init daemon有两种一种是SysV 的方式，一种是Upstart的方式(ubuntu)

###Sys V
+   主要目录是/etc/init.d/ /etc/rc?.d/
    init.d/ 中是配置文件 /rc?.d 中都是对于的链接文件
+   rcX.d/ 是这个里面都是指向实际配置脚本的链接，名字是以K S开头
    加上一个优先级的数字+daemon的名字。 
    K是kill，当机器运行级别下降是会执行K开头的，
    S是start，当机器运行级别上升是会执行S开头的
+   使用 update-rc.d 可是设置配置文件 see man page
+   service 一个控制SysV的 init的工具

###Update
+   ubuntu使用Upstart管理启动进程
+   /lib/init/upstart-job 就是Upstart 的配置文件
    所有链接到这个文件的文件都使用Upstart 管理模式
+   chkconfig 一个设置启动服务的工具
+   所有的配置文件在/etc/init 下面，通过事件驱动。
+   兼容老的SysV 方式 
+   see man upstart


###more see
The Linux programming interface


