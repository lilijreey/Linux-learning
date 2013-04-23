GRUB2
========================================
ubuntu 使用grub2 

### 最佳实践
*  开启gurb 界面
      1. 修改 /etc/default/grub GRUB_HIDDEN_TIMEOUT=5 改为非0值
      2. 修改 /etc/grub.d/30_uefi-firmware 把文件中的所有涩痛timeout=0 改为非0 值

*  图片背景
   

### Files 
  /etc/grub.d 模版文件
  /etc/default/grub //设置文件

  /boot/grub/grub.cfg 根据上面的配置文件自动生成的

### optins 选项说明
  insmod xxx 插入指定模块

  set default="0" 
  set root=(hd0,8) 指定/分区
  search --no-floppy --fs-uuid --set=root a8be5597-da20-4ceb-bc6f-ba2d2ae5ae92
      指定uuid=a8be5597-da20-4ceb-bc6f-ba2d2ae5ae92 的分区为root分区
  如果上面'set root(hd0,8) 的uuid和这里的uuid一致则两句作用相同，如果不一致
  则 search 为准
  set gfxmode=auto 设置显示分辨率 可用 640x480 800X600 1024X768
  insmod dfxterm 支持中文和24为图片

   //设置菜单的字体和被景色
    set menu_color_normal=white/black
    set menu_color_highlight=black/light-gray

   //菜单选项 每个菜单选项都要 有 menuentry 'tital' {set ...} 组成
    menuentry 'Ubuntu, with Linux 3.2.0-40-generic' --class ubuntu --class gnu-linux --class gnu --class os {
        recordfail
        gfxmode $linux_gfx_mode
        insmod gzio
        insmod part_gpt
        insmod ext2
        set root='(hd0,gpt2)'
        search --no-floppy --fs-uuid --set=root a8be5597-da20-4ceb-bc6f-ba2d2ae5ae92
        linux	/boot/vmlinuz-3.2.0-40-generic root=UUID=a8be5597-da20-4ceb-bc6f-ba2d2ae5ae92 ro   
        initrd	/boot/initrd.img-3.2.0-40-generic
    }
 
### grub 命令
* update-grub 更新/boot/grub/grub.cfg 文件 一般在更新设置后


### 设备命名规则
和linux的设备命名规则不同 
 第一块硬盘 /dev/sda  第二块/dev/sdb
          分区从1开始 四个主分区1-4
          逻辑分区从5开始

grub 使用 fd表示软盘
          hd表示硬盘
          设备从0开始编号;
          分区从编号和linux相同
  如第一块磁盘的1号主分区
  GRUB      Linux
  (hd0,1)   sda1  0=a 代表第一个设备 
  (hd0,5)/boot/vmlinux 表示(hd0,5)逻辑分区下boot目录的vmlinux文件

  more see grub2 基础教程 web page
