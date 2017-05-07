## ELF格式文件的连接和加载

## ELF
elf 是有格式的二进制文件
包括 ELF 头, 数据
elf 有两种组织方式,一种是segments, 一种是Sections
一个ELF 可以有0到多个segments
readelf -l 查看所有segment信息


## 查看ELF文件的工具
* elfutils
  包含eu- 开头的一系列工具

see readelf.reamdme


### 连接
* 作用
把多个文件合并为一个文件,如何布局,符号的地址

* 每个obj文件,有一些section,
* section, 每一个section有一个名字和大小
  大多数section有一个关联的数据块.叫做section contents
  一个section可能被标记为'loadable',意味着在执行这个程序时,这个section需要被加载到内存中
  如何一个section没有contetnts就很有可能是'allocatable',意味着需要分配,通常是.bss
  如果一个section,不是上面的两种那么通常会包含一中调试信息
  每一个loadable,allocatable Section都有两个地址,一个是VMA(虚拟地址),在程序运行是的段地址
   一个是LMA(加载地址),大多数情况下这两个地址是相同的. 一种不同的情况为,加载到一个ROM系统,
   但是需要一份拷贝到RAM系统执行

   使用 objdump -h 可以查看sections
   

## 连接脚本
连接脚本是一个文本文件,里面包含一系列的命令. 每一个命令都有一个Keyword,也许还带有参数,或者
 符号赋值. 不同的语句用分号隔开

* 连接脚本功能
1. 指定输出的可执行文件格式,架构,程序入口点

### 命令
```
SECTIONS
{
    . = 0x10000;
    .text : { *(.text) }
    . = 0x8000000;
    .data : { *(.data) }
    .bss : { *(.bss) }
}
SECTIONS 用来声明输出文件的内存布局
SECTIONS 里面包含一系列的符号赋值和输出section声明在大括号里
'.'' 的意思是指定当前地址,也就是说.text Section 从0x10000开始
.text : {*(.text)} 的意思是把所有input obj文件中的*.text 汇总到output
 文件的.text Section中 * 是通配符, 匹配所有的input File name

input文件的section 还可以这样写
 file.o(.data) 一个特定文件的.data
 *(.data, .rdata)
 SORT_BY_NAME(.text*) 对匹配的section排序

ENTERY (symbol)
设定程序入口
There are several ways to set the entry point. The linker will set the entry point by trying
each of the following methods in order, and stopping when one of them succeeds:
• the ‘-e’ entry command-line option;
• the ENTRY(symbol ) command in a linker script;
• the value of the symbol start, if defined;
• the address of the first byte of the ‘.text’ section, if present;
• The address 0.


INPUT(file, file)
 设定连接文件, 不用每次都在命令行指定

OUTPUT(filename)
like -o

SEARCH_DIR(path)
like -L path


对象格式命令
OUTPUT_FORMAT(default, bigEnd, littleEnd)
 指定二进制格式,默认,大端系统,小端系统
OUTPUT_ARCH(bfdarch) 指定架构


赋值
symbol = expression; 定义一个符号
e.g. xx=0x32342311;
     buf= (. + 16)

PROVIDE(symbol=exp)
 如果没有obj文件定义这个符号,就使用这里定义的


### 从源码中访问连接脚本定义的符号(地址)
extern int foo;

### BFD
是obj 文件的格式.
linux支持多种格式,使用 objdump -i 显示所有支持的格式
