/*
 * =====================================================================================
 *
 *       Filename:  device.c
 *
 *    Description:  linux Device notes
 *
 *        Created:  04/08/2013 10:20:15 AM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */

/*
 * Linux 用两个数字来标记 Devices, 主设备号，和从设备号，
 * Major device number 指定哪个驱动（driver)和这个设备关联
 *      所有的主设备号和设备的关联都是固定的是内核源码的一部分
 *      /usr/src/linux/Documentation/devices.txt (linux src)
 *      /proc/devices 列出所有已经载入内核的主设备号和驱动
 * Minor device number 用来区分同一主设备号的不同设备，
 *      从设备号的含有和具体的驱动有关
 *
 *  几个特殊设备
 *  /dev/null 主要有两种作用
 *    1. 把不需要的输出写到/dev/null, Linux 丢弃所有写到/dev/null设备的数据
 *    2. 从/dev/null 中读取， 当从/dev/null中读取数据时总是返回EOF
 *      如果你把/dev/null 复制过来会得到一个空文件
 *  /dev/zero 
 *      这个设备的行为像是由无限的0bytes填充的，
 *      如果从/dev/zero 中读取数据就会读入任意产度的0bytes
 *      常用 mmap mapping /dev/zero 来分配内存
 *  /dev/full
 *      行为是像一个无法在写入的文件，如果向full写入数据就会收到一个ENOSPC
 *      的错误， 常用来测试软件在无法写入时的行为
 *
 *
 */

