# 学习 GCC COMILPER 特性的笔记


-S 生成汇编代码

## 生成源代码汇编注释版本
gcc -c -g -Wa,-adlhn ee.c > outfile

或者是生成 -g 的程序后
使用 objdump -d -S xx.o

-I dir 添加头文件搜索路径
-isystem  添加系统文件搜索路径
  e.g.
	g++ -Wall -isystem . -std=c++14 -o tt test.cpp 

## builitins.c 
> GCC __builitin系列函数笔记


     -ggdb3
     -g3 可以包含宏信息 在gdb中使用 info macro <name> 察看

+ 关闭某些警告
   -Wno-<XXXX>

+ -funroll-loops
   完全展开循环结果 可以用来抵制反汇编

+ -Wformat=2
   -Wall 中包含Wformat=0
	   -Wformat-nonliteral,
 如果fmt 不是字符串常量，就警告 -Wall 不包含这个警告选
       -Wformat-security, 
       -Wformat=2  include 上面的两个
        are available, but are not included in -Wall.

+ -Wabi

+ -fdump-class-hierarchy
  会输出所有class 的成员sizeof, 偏移, 以及成员函数, 和类的继承关系
  到一个文件中, 对学习C++ class 很有帮助

+ -fdump-tree-cfg
  会输出在编译器添加辅助代码后的程序
  到一个文件中, 对学习C++ class 很有帮助


   -fdump 指令??

##  <compiler.h> or /usr/lib/syslinux/com32/include/klibc/compiler.h
    定义了许多特性的宏

## GCC 内置函数
+ builitins.c 
------------------------------
  GCC `__builitin系列函数笔记`

+ `__sync_*` suit functions
------------------------------
   since GCC 4.1 support `__sync_*` built-in func
    用于原子的对加减和逻辑操作

+ gcc 使用的stack-guard 技术
------------------------------
    gcc使用 ssp 保护stack, 默认不开启
      +  -protector：
        启用堆栈保护，不过只为局部变量中含有 char 数组的函数插入保护代码。

      + -fstack-protector-all：
        启用堆栈保护，为所有函数插入保护代码。

      + -fno-stack-protector：
        禁用堆栈保护。

## GCC-C 扩展特性
-------------------------------------
+  GCC 支持对void和函数指针的算术操作
   对上面两种指针加1的语意是增加1字节.

+  switch 
    case 1 ... 3:
    必须有空格

+  零元素数组 int [0]

+  gcc 把() 中的符合句作为一个表达式对待，称为语句表达式
    e.g. #define min_t(x, y) ({int _x = x; int _y = y; _x < _y? _x : _y>})
    mini = min_t(a, b);

+  typeof 关键字
    typeof(exp) 可以获得x的类型
    e.g. const typeof(x) _x= (x);

+  可变参数宏 
     e.g. #define debug(fmt, arg...)  printk(fmt,##arg)
          当arg为0个参数时， ##会把逗号去掉

+  预定义当前函数名 __FUNCTION__ C99 已经提供__func__ 和


## pragma
>  pragma once 一次引入 编译器提供

## map 文件
>  MAP 文件是程序的全局符号、源文件和代码行号信息的唯一的文本表示方法，
   是整个程序工程信息的静态文本，通常由linker生成。 
   注：map文件在各种编译器中都可以使用，不限于gcc.
   
   +. 生成map文件,需要obj携带符合表
      gcc -Ml,-Map,file.map -g file.c -o file 
      dl -Map file.map
   +. 查看指定函数的名字-地址映射
      grep func file.map


