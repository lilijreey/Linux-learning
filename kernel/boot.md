linux 系统引导
================================

1. 当系统上电或复位时， CPU会吧PC寄存器设置为 *0xFFF0* 
   并从该地址执行指令。 在PC机上该地址位于BIOS中

2. BIOS 按照COMS 中的设定来查找可引导的设备，(就是bios中的boot 选项)
    若是从硬盘引导， BIOS会将硬盘上的MBR 加载到RAM中， 然后将控制权
       让给MBR

3. 主引导程序会查找并加载从引导程序，
  Qus. GRUB 是从引导程序吗？

4. 从引导程序加载linux内核和可选的RAM磁盘(/boot/initrd.img-3.2.0-38-generic),
   并将控制权交给linux内核

5. kernel(/boot/vmlinuz-3.2.0-38-generic) 自解压部kernel
   `start() -> startup_32() -> start_kernel() -> cpu_idle()
    

Bootloader
==================================
能做系统上电或复位的时候一某中方式执行
能将U盘， 磁盘， 光盘， Flash SD 网卡 等介质中的
操作系统加载到RAM，并把控制权交给OS

常见的linux Bootloader 有 LILO,GRUB,U-Boot 等
