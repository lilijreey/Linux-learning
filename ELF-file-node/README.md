# Persenol leanning elf object construction symbol link and load, and GNU binutils

## 链接脚本
1.  ld在出处理各种目标文件时，总是根据一系列规则产生输出.
     这些规则被称作链接脚本.  脚本一般安装在*/usr/lib/ldscripts/*
2.  ld -T file.script 制定链接脚本
3.  使用链接脚本可以控制输出文件中的段，加载地址等等属性

## ldd

## ldconfig
set ld search path and cache
 -v shwo all of search path and shared-libs
 -X rebuild /etc/ld.so.cache

## ld.so 
see man-page

## ELF file format
`
so file:
    NAME                    SIZE          COMMENT
<Begin>
1.  ELF Header              52            Elf32-Ehdr
<紧接着> see -h Start of program headers:
2.  Program-Header-Tab   number*size     see -h Size of program headers  
                                                 Number of program headers
<紧接着> 
3.  Section                              see -S Off
...
4.  Sections-headers-Tab  number*size     see -h Size of section headers:   
                                                 Number of section headers: 
                                          address see -h Start of section headers
<End>

conceptions:
1.  Program header: ??
2.  Section header: ??

EE watch elf info
1. 察看ELF文件头 可以知道大致的elf文件信息  readelf -h a.so
ELF Header:
  Magic:   7f 45 4c 46 01 01 01 00 00 00 00 00 00 00 00 00 
  Class:                             ELF32
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              DYN (Shared object file)
  Machine:                           Intel 80386
  Version:                           0x1
  Entry point address:               0x360 运行程序的地址， 对于可重定位文件该值为0
  Start of program headers:          52 (bytes into file)   Program-headers-tabl address in elf offest
  Start of section headers:          4372 (bytes into file) Section-headers-tab 在elf文件中的起始位置
  Flags:                             0x0
  Size of this header:               52 (bytes) elf header size
  Size of program headers:           32 (bytes) 每个program-headers 结构的大小(sizeof Elf32-Shdr)
  Number of program headers:         6 给ELF文件有几个program-headers
  Size of section headers:           40 (bytes)
  Number of section headers:         27
  Section header string table index: 24 .shstrtab 段在段表中的下标 

EE readelf -S elffile 列出所有section-headers table 里面包含所有 headers
输出的顺序就是每个header 在tab中的下标顺序.
第一行现实Section-headers 的个数，和table的其实地址。（offset 0x1114 相对于elf文件的偏移地址)
Name: 所有的这些字符都在.shstrtab Section 中保存，每个名字一NULL结尾. 
      struct 中存放的是name-str的offest in .shstrtab
Addr: 加载程序后在virtal memroy中的地址， 如果段不能被加载则是 0
Off: 每个section在elf的偏移地址(开始地址)
Size: size of section
Type: type of section
   NULL: 无效段 NOTE：提示信息 PROGBITS： 程序段 SYMTAB： 符号段 STRTAB: 字符串段
   RELA：重定位段 REL:  重定位段 DYNAMIC: 动态链接段 SHLIB： 保留 GNU_HASH: 符号表的hash表
   VERSYM:  VERNEED:    <see elf.h>
`
    There are 27 section headers, starting at offset 0x1114: (byte)
    Section Headers:
      [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
      [ 0]                   NULL            00000000 000000 000000 00      0   0  0
      [ 1] .note.gnu.build-i NOTE            000000f4 0000f4 000024 00   A  0   0  4
      [ 2] .gnu.hash         GNU_HASH        00000118 000118 000034 04   A  3   0  4
      [ 3] .dynsym           DYNSYM          0000014c 00014c 0000a0 10   A  4   1  4
      [ 4] .dynstr           STRTAB          000001ec 0001ec 00007c 00   A  0   0  1
      [ 5] .gnu.version      VERSYM          00000268 000268 000014 02   A  3   0  2
      [ 6] .gnu.version_r    VERNEED         0000027c 00027c 000030 00   A  4   1  4
      [ 7] .rel.dyn          REL             000002ac 0002ac 000020 08   A  3   0  4
      [ 8] .rel.plt          REL             000002cc 0002cc 000018 08   A  3  10  4
      [ 9] .init             PROGBITS        000002e4 0002e4 000030 00  AX  0   0  4
      [10] .plt              PROGBITS        00000314 000314 000040 04  AX  0   0  4
      [11] .text             PROGBITS        00000360 000360 000128 00  AX  0   0 16
      [12] .fini             PROGBITS        00000488 000488 00001c 00  AX  0   0  4
      [13] .rodata           PROGBITS        000004a4 0004a4 000005 00   A  0   0  1
      [14] .eh_frame         PROGBITS        000004ac 0004ac 000004 00   A  0   0  4
      [15] .ctors            PROGBITS        00001f14 000f14 000008 00  WA  0   0  4
      [16] .dtors            PROGBITS        00001f1c 000f1c 000008 00  WA  0   0  4
      [17] .jcr              PROGBITS        00001f24 000f24 000004 00  WA  0   0  4
      [18] .dynamic          DYNAMIC         00001f28 000f28 0000c0 08  WA  4   0  4
      [19] .got              PROGBITS        00001fe8 000fe8 00000c 04  WA  0   0  4
      [20] .got.plt          PROGBITS        00001ff4 000ff4 000018 04  WA  0   0  4
      [21] .data             PROGBITS        0000200c 00100c 000004 00  WA  0   0  4
      [22] .bss              NOBITS          00002010 001010 000008 00  WA  0   0  4
      [23] .comment          PROGBITS        00000000 001010 00002b 01  MS  0   0  1
      [24] .shstrtab         STRTAB          00000000 00103b 0000d8 00      0   0  1 <section-string-tab
      [25] .symtab           SYMTAB          00000000 00154c 000350 10     26  44  4 <symbol-tab>
      [26] .strtab           STRTAB          00000000 00189c 00017b 00      0   0  1 <string-tab>
    Key to Flags:
      W (write), A (alloc), X (execute), M (merge), S (strings)
      I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)
      O (extra OS processing required) o (OS specific), p (processor specific)
  `

## ELF 文件中的符合表
 往往是一个section， 通常都是.symtab. 用来管理没elf文件中的所有符号
 .symtab 是一个symbol-node的数组， symbol-node type is Elf32_Sym
 .dynsym 是一个动态synbol-node 数组
 
 readelf -s Displays the entries in symbol table section of the file, if it has one.
 比nm还要全面，会现实没有符号的node



Num: 在section从的下标
Value: 符号值
Size：符号大小
Type: 符号类型
    NOTYPE 没有类型 SECTION 段符号 FUNC    函数符号 FILE    文件符号
    OBJECT  ？？？
Bind: 绑定类型。 
      Local 本地，只能在lib中可见，对用链接库，应用程序无法看到local的符号。
            可是使用version-script file 指定符号的bind 类型
      global 全局，
            global符号对外可见，so导出的符号需要是global的. 全局符号默认为global
      week弱,  

Vis： 符号的可视性 使用__attribute((visibility()) 属性可以指定，一般都是DEFAULT
Ndx： 符号所在的section。 section-headers-tab 中的下标
Qus. 绑定类型和可是性的区别

Symbol table '.dynsym' contains 10 entries:
   Num:    Value  Size Type    Bind   Vis      Ndx Name
     0: 00000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 00000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
     2: 00000000     0 NOTYPE  WEAK   DEFAULT  UND _Jv_RegisterClasses
     3: 00000000     0 FUNC    GLOBAL DEFAULT  UND puts@GLIBC_2.0 (2)
     4: 00000000     0 FUNC    WEAK   DEFAULT  UND __cxa_finalize@GLIBC_2.1.3 (3)
     5: 00002018     0 NOTYPE  GLOBAL DEFAULT  ABS _end

Symbol table '.symtab' contains 53 entries:
   Num:    Value  Size Type    Bind   Vis      Ndx Name
     0: 00000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 000000f4     0 SECTION LOCAL  DEFAULT    1 
     2: 00000118     0 SECTION LOCAL  DEFAULT    2 
     3: 0000014c     0 SECTION LOCAL  DEFAULT    3 
     4: 000001ec     0 SECTION LOCAL  DEFAULT    4 
     5: 00000268     0 SECTION LOCAL  DEFAULT    5 
     ...
    22: 00002010     0 SECTION LOCAL  DEFAULT   22 
    23: 00000000     0 SECTION LOCAL  DEFAULT   23 
    24: 00000000     0 FILE    LOCAL  DEFAULT  ABS crtstuff.c
    25: 00001f14     0 OBJECT  LOCAL  DEFAULT   15 __CTOR_LIST__
    26: 00001f1c     0 OBJECT  LOCAL  DEFAULT   16 __DTOR_LIST__
    27: 00001f24     0 OBJECT  LOCAL  DEFAULT   17 __JCR_LIST__
    28: 00000360     0 FUNC    LOCAL  DEFAULT   11 __do_global_dtors_aux
    29: 00002010     1 OBJECT  LOCAL  DEFAULT   22 completed.7065
    30: 00002014     4 OBJECT  LOCAL  DEFAULT   22 dtor_idx.7067
    31: 000003e0     0 FUNC    LOCAL  DEFAULT   11 frame_dummy
    32: 00000000     0 FILE    LOCAL  DEFAULT  ABS crtstuff.c
    33: 00001f18     0 OBJECT  LOCAL  DEFAULT   15 __CTOR_END__
    34: 000004ac     0 OBJECT  LOCAL  DEFAULT   14 __FRAME_END__
    35: 00001f24     0 OBJECT  LOCAL  DEFAULT   17 __JCR_END__
    36: 00000450     0 FUNC    LOCAL  DEFAULT   11 __do_global_ctors_aux
    37: 00000000     0 FILE    LOCAL  DEFAULT  ABS a2.c
    38: 0000041c    38 FUNC    LOCAL  DEFAULT   11 a
    39: 00001ff4     0 OBJECT  LOCAL  DEFAULT  ABS _GLOBAL_OFFSET_TABLE_
    40: 0000200c     0 OBJECT  LOCAL  DEFAULT   21 __dso_handle
    41: 00001f20     0 OBJECT  LOCAL  DEFAULT   16 __DTOR_END__
    42: 00000417     0 FUNC    LOCAL  DEFAULT   11 __i686.get_pc_thunk.bx
    43: 00001f28     0 OBJECT  LOCAL  DEFAULT  ABS _DYNAMIC
    44: 00000000     0 NOTYPE  WEAK   DEFAULT  UND __gmon_start__
    45: 00000000     0 NOTYPE  WEAK   DEFAULT  UND _Jv_RegisterClasses
    46: 00000488     0 FUNC    GLOBAL DEFAULT   12 _fini
    47: 00002010     0 NOTYPE  GLOBAL DEFAULT  ABS __bss_start
    48: 00002018     0 NOTYPE  GLOBAL DEFAULT  ABS _end
    49: 00000000     0 FUNC    GLOBAL DEFAULT  UND puts@@GLIBC_2.0
    50: 00002010     0 NOTYPE  GLOBAL DEFAULT  ABS _edata
    51: 00000000     0 FUNC    WEAK   DEFAULT  UND __cxa_finalize@@GLIBC_2.1
    52: 000002e4     0 FUNC    GLOBAL DEFAULT    9 _init
