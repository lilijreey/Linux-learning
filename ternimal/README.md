关于 tty, pty, 实现

* 以前用来访问UNIX需要通过ternimal,就是一个CRT电子射频显示器,通常的大小是80x24
* 早期终端不是标准设备,有大量的型号不同的终端. 主要这时的是真是设备.
* 我们现在用的都是软件编写的终端模拟器. e.g.xterm
* 不管是真实设备还是模拟器都是处理输入输出
* 终端有多种模式,输入模式,显示模式,控制字符... 可以使用stty 进行查询和设置

see pty(7) pts(4) ttyname(3)


一个终端设备通常使用/dev/ttyX 来表示

## Console, Terminal, VTermianl, 终端模拟器, 伪终端, Shell, Programm 关系
* Console 
  设备
  计算机自带的控制台,含有终端的功能,但是有些信息只会显示在console上,比如内核消息
  linux对应 /dev/console 这个文件系统控制终端. 通常会把console映射到当前的tty上

* terminal 
  设备
  一个电脑只有一个Console, 但是为了多人共享这个电脑,可以使用terminal连接电脑
  以前终端想一个打字机,输出被打印到纸上,有了显示器后,在把输出显示出来. 二战时就有了.

* tty (TeleTYpewriter)
  设备 tty 也叫终端
  没有电子终端前的终端. TTY的名字一直保留了下来. 
  teleprinter, 现在都是程序实现的/dev/tty

* Virtual Terminal
  虚拟设备
  使用程序模拟的终端,并不是物理终端. 比如现在的tty
  出现原因,不需要多个物理终端,也能和计算机进行交互


* pty 伪终端
  程序, 为了实现一些程序
  一个成对的罗辑设备.
  /dev/pts/ptmx 主设备
  /dev/pts/ 从设备配合实现pty

  分为master和svlea 对其中一个端的读写会反应到另一端
  像是一个管道,两个程序可以通过为终端进行通信
  通常程序首先打开主设备,然后fork, 子进程打开从设备,重定向IO,然后调用exec

  调用posix_openpt(int flags) 会产生一对主从设备,并返回主设备句柄
   本质上是 open("/dev/ptmx", flags);
   ptsname(masterfd 得到一个从设备的名称

* 终端模拟器
  XWindos 中的为终端 e.g xterm
  上面的概念都是为大型机,小型机准备的.当在PC上实现unix的时候,还是保留了
  原来的结构.不过终端从一个物理设备变成了由程序模拟

* shell
  是内核的上层和上面的概念完全不同.上面的都是和计算机的输入输出相关.

* /dev/tty 用于是当前程序所在的终端

* 终端类型
  echo $TERM
  infocmp  vt100 显示终端特性

* TERM 环境变量
  应用程序通过访问TERM就可以指定终端的支持的特性


