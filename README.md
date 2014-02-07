### 个人学习linux sys call + C stander lib 的笔记

#include <signal.h>
###几种代码规范
在代码的最开始定义
`
+   _GNU_SOURCE
+   _BSD_SOURCE
+   _SVID_SOURCE
`

### files
+  device.c linux 设备学习笔记
+  base64.c base64 编码实现
+  GNUCLIB.c  :GNU C lib 提供的非标准的特性功能
+  `exec_calls.c` exec 系列函数
+  sysconf.c sysconf 函数详解
+  container/ data structer 提供的数据结构
      queue. /sys/queue.h In BSD

### system call
+  为了安全性，user-spase 不能直接调用kernel-spase， user-spase
   只能给通知kerenl来让kernel来为自己执行内核提供的功能，
+  liunx 通过 int 80 cpu 指令来陷入内核，
   所有的系统调用都通过一个数字来表示，通过eax,制定
   调用参数通过ebx,ecx,edx,esi,edi,来传递前5的参数，如果还有通过stack传递
