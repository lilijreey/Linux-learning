linux 内存管理
===============================


X86-64 保护模式
-------------------------

MMU
---------------------------
cpu 硬件内存管理
TLB 属于MMU的一部分

寻址方式
--------------------------------
在保护模式下cpu发出的是线性地址， MMU根据CR3 Register指向的页表
讲线性地址转换为物理地址.

* 每个进程有自己的页表
* 所有进程的内核页表地址相同
* 进行切换时会刷新TLB(translation lookaside buffer)
  TLB 就是加速虚拟地址->物理地址查找的cache 表

线性地址本身分为很多字段，每一段都是页表中的选择子。
* 4级页表
    47 39-9bit PageMap MPL4T
    38 30-9bit Page Directory Pointer
    29 21-9bit Page Table Selector
    20 12-9bit Page Table Entry
    11 0-12bit 页内字节寻址

    64位体系中只实现了48位的虚拟地址。 高16位被用在符号扩展，要么都是0，要么都是1 (Qus. 为啥??)
    普通页的大小为4k， 但是一个地址为64bit。 所以一个页中能有512项内容, 由于2^9 = 512，
    所以可以看到线性地址中前四级页表选择子的宽度都是9bit, 因为美级页表都要对齐,都是4k大小

    由于48bit的线性地址，用9bit来划分，所以分为了4x9=36 + 11. 为4级页表，32是2级

* 可选的其他转换方式
   根据物理页的大小划分
    2M =4 k * 512 页表转换 PML4T +  PDPT + PDT | 21 为的页内指针 
    1G = 

* 最大可寻址范围
   使用 MAXPHYADDR 宏定义主要由
   36bit 40bit 52bit


内核物理内存布局
==========================================
+------------------------------------------+
|       用户空间      |   内核空间         |
+------------------------------------------+
0                     3G                  4G

x86-32 内核1G分为3个区域:
            0           16MB              896MB
            +------------------------------------------------+
            |  ZONE_DMA  |  ZONE_NORMAL    |  ZONE_HIGHIMEM  |
            +------------------------------------------------+
            3G           内核空间                           4G

* ZONE_DMA  直接映射，因为ISA总线只能对前16MB进行寻址，因为总线地址为24位地址(16位ISA)
* ZONE_NORMAL 通过OFFSET 直接映射，即可得到线性地址对应的物理地址
* ZONE_HIGHIMEM 用来访问大于896的物理地址(映射)


x86-64
+------------------------------------------+
|       用户空间      |   内核空间         |
+------------------------------------------+
0      用户空间      128T  内核空间     256T

内核空间结构
ff800000000000 - ffff80ffffffffff (=40 bits) guard hole
ff880000000000 - ffffc7ffffffffff (=64 TB) direct mapping of all phys. memory
ffc80000000000 - ffffc8ffffffffff (=40 bits) hole
ffc90000000000 - ffffe8ffffffffff (=45 bits) vmalloc/ioremap space
ffe90000000000 - ffffe9ffffffffff (=40 bits) hole
ffea0000000000 - ffffeaffffffffff (=40 bits) virtual memory map (1TB)
. unused hole ...
ffffff80000000 - ffffffffa0000000 (=512 MB)  kernel text mapping, from phys 0
ffffffa0000000 - fffffffffff00000 (=1536 MB) module mapping space


内存分段信息
   /proc/zoneinfo


kernel 内存分配接口
======================================
* kmalloc kfree  kalloc  krealloc
 动态大小内存分配, 在3G高空间分配，与物理地址一一对应
  访问速度较快

* vmalloc
   在 VMALLLOCSTAERT~4GB 分配
   kmalloc 保证物理地址连续，vmalloc不保证,主要用在对内存访问要求不高的场景

* kmem_cache_create kmem_cache_destroy
* kmem_cache_alloc kmem_cache_free
  Slab 固定大小内存分配,内存池分配

* alloc_pages/free_pages
  buddy 2^n次方分配,页分配


* dma_alloc_coherent
   分配DMA内存保证内存与cpu缓存的数据一致性。
   X86系统上硬件已经保证了, 底层使用alloc_pages

* ioremap
  直接把物理地址映射到内核空间。 物理地址一般都是事先确定好的，
  主要用于设备驱动，可以让cpu访问外部设备的IO空间

* alloc_bootmem
  如果要分配大量连续物理内存，


内存管理算法
=========================================
* subbdy 页分配算法 
   /proc/subbdyinfo
* slab 对象池
   /proc/slabinfo
