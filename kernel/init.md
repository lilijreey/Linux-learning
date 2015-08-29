init 程序
负责服务的启动，准备工作

几种主要的init　程序
init 是user space 中的第一个程序
1. systemv
  所有服务只能开机启动，不支持动态启动

2. Upstart
   based-on event

3. systemd

   systemd 是一个体系包括内核的支持，应用层的可编程接口和API
     包括系统服务管理
     软件平台
     用户层和内核层的胶水

     核心组件

    + systemd is a system and service manager for Linux operating systems.
    + systemctl may be used to introspect and control the state of the systemd system and service manager.
    + systemd-analyze may be used to determine system boot-up performance statistics and retrieve other state and tracing information from the system and service manager.
