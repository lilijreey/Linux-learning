## Bochs
一个x86 模拟调试题

* install 不能使用apt 直接安装，需要到官网下源码
  bochs-sdl bochs-x bochs

* ./confui --enable-debugger --enable-dismiss
   基本上编译不过

* config a VM
  touch bochsrc
bochsrc <<< EOF  

megs:32

romimage: file=/usr/share/bochs/BIOS-bochs-latest
vgaromimage: file=/usr/share/vgabios/vgabios.bin

floppya: 1_44=a.img, status=inserted

boot: floppy

log: bochs.log
mouse: enabled=0
display_library: sdl

keyboard_mapping: enabled=1, map=/usr/share/bochs/keymaps/x11-pc-us.map


 EOF


 安装 OS
1. make boot img
  bximage
2. write boot.bin to img
   dd if=boot.bin of=a.img bs=512 count=1 conv=notrunc

