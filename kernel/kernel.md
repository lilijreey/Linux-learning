linux kernel 学习
==============================

## 编译内核

1. 配置内核
---------------------
  arch/xxx/configs/ 下有各个平台的默认的配置文件;

  默认ARCH=ai64
  使用 make ARCH=xxx 来交叉编译

* make config 基于文本界面 不好
* make menuconfig 基于文本菜单 推荐 需要安装 ncurses lib
   按 空格 或 Y N 选择
* make xconfig  QT GUI
* make gconfig  GTK+ GUI
  配置或会生成一个.config 文件


2. 编译内核
-------------------------
make bzImage 
   生成 vmlinux
        System.map

make modules 编译内核模块
   生成 Module.symvers
        modules.builtin
        modules.order
        其他mod 文件

3. 安装内核
------------------------
`make modules_install
make install`
 
安装编译的内核和模块到 /boot  /lib/modules/xxx

4. 更新grub
------------------------
update-grub

5. 重启 就好了


## /boot file详解 
* vmlinuz-3.2.0-38-generic
          是可执行的压缩内核

* config-3.2.0-29-generic 
        内核编译时的.config 文件

* System.map-3.2.0-29-generic
     内核符合映射表 通过 nm vmlinuz 产生

* initrd.img-3.2.0-29-generic
     initial ramdisk 的缩写  用来引导 kernel mount真正的文件系统
       是一个辅助工具
    使用 mkinitrd 工具创建 
* abi-3.2.0-38-generic




make 选项
-----------------------------
make help 察看帮助
make mrproper 把原来编译产生的垃圾清除
make clean    类似于mrproper 保留.config

