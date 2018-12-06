## UIO 用户态IO 学习笔记

内核支持UIO,默认不安装
内核默认提供了多种uio驱动
cat /boot/config | grep uio 查看内核是否支持

modprobe uio_pci_generic
需要主动load ,分配id并绑定设备
echo "8086 10f5" > /sys/bus/pci/drivers/uio_pci_generic/new_id

## 绑定设备
Linux内核从2.6.13-rc3开始，提供了在用户空间，可动态的绑定和解绑定设备和设备驱动之间关系的功能。
只需要把当前设备id,写入绑定驱动的unbind 文件中即可
1. 需要先解绑设备驱动
  lspci 找一个usb设备
  找到该设备对应的驱动/sys/bus/pci/drivers/<xx>/
  echo <pciid> > /sys/bus/pci/drivers/<xx>/unbind

2. 把设备绑定到uio_pci_generic 驱动上

[doc](https://www.kernel.org/doc/html/v4.12/driver-api/uio-howto.html)


## 编写uio 内核模块
see myuio_ker.c
 申请了1024内核内存，用于用户側直接访问
 make; insmod myuio_ker.ko
  安装之后会生成/dev/uio0, 
                /sys/class/uio/uio0 下面有uio的多种信息可以读取
                
