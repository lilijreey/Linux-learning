/*
 * =====================================================================================
 *
 *       Filename:  device.c
 *    Description:  linux Device notes
 *        Created:  04/08/2013 10:20:15 AM
 *        深入解析见 kernel/moudle
 *        Linux 支持的设备  www.lanana.org/docs/device-list
 * =====================================================================================
 */

/**
 * Qus. 如何察看设备相关连的模块和驱动
 *      1 察看设备的主设备号
 *      2 从 /proc/moudle 查询 
 *
 * Linux 用两个数字来标记 Devices, 主设备号，和从设备号，
 * Major device number 指定哪个驱动（driver)和这个设备关联
 *      所有的主设备号和设备的关联都是固定的是内核源码的一部分
 *      /usr/src/linux/Documentation/devices.txt (linux src)
 *      /proc/devices 列出所有已经载入内核的主设备号和驱动
 * Minor device number 用来区分同一主设备号的不同设备，
 *      从设备号的含有和具体的驱动有关
 *      
 *  几个工具 1/proc/interrupts 查看终端号
 *            /proc/pci/
 *            
 *          2 lspci -v
 *      
 *
 *  几个特殊设备
 *  /dev/null 主要有两种作用
 *    1 把不需要的输出写到/dev/null, Linux 丢弃所有写到/dev/null设备的数据
 *    2 从/dev/null 中读取， 当从/dev/null中读取数据时总是返回EOF
 *      如果你把/dev/null 复制过来会得到一个空文件
 *  /dev/zero 
 *      这个设备的行为像是由无限的0bytes填充的，
 *      如果从/dev/zero 中读取数据就会读入任意产度的0bytes
 *      常用 mmap mapping /dev/zero 来分配内存
 *  /dev/full
 *      行为是像一个无法在写入的文件，如果向full写入数据就会收到一个ENOSPC
 *      的错误， 常用来测试软件在无法写入时的行为
 *
 *  /dev/mem
 *    物理内存的全映像。
 *    可以用来直接访问 物理内存,而不虚拟内存
 *    一般的用法是 open("/dev/mem", O_RDWR|O_SYNC)
 *     mmap(xxx
 *     使用/dev/mem 可以实现用户空间驱动
 *
 *  /dev/random
 *      真随机数产生设备
 *  /dev/urandom
 *     速度较快的随机数产生设备
 *
 *  /dev/fd0 软驱
 *  /dev/ram0 RAM disk
 *  /dev/hda IDE 的第一个设备
 *  /dev/sda SCSI 的第一个设备
 *  /dev/disk 存放连的到主机上的硬盘信息，按照不同的方式共用户查询
 *  /dev/tty 用户当前正在使用的tty
 *  /dev/ttyS0 COM1
 *     可以使用 stty -F /dev/ttyS0 来查看串行端口的通信参数
 *  /dev/console 系统终端接口
 *  /dev/input 输入设备
 *            /mice 鼠标设备
 *  /dev/mapper LVM 机制使用
 *  /dev/net/tun 用来建立VPN 的trunnel
 *  /dev/pts/  主机上所有打开的模拟终端， 打开一个gnome-terminal 就会产生一个
 *   
 *
 */

