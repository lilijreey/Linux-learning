/**
 * @file     garbage_collect.c
 *           GC  
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     14/10/14 12:31:15
 *
 * 使用ＧＣ的前提
 *
 * GC的几种算法
 * 
 *   * 标记-清扫式(mark-sweep)
 *    程序变量和堆分配的记录构成一个有向图，
 *    每个程序变量是图中的一个根，如果存在从某个
 *    根节点R出发，沿着又向边到达一个N，则称节点N
 *    是可达的。类似于深度优先算法
 *    
 *     DFS(x)
 *        if is_haep_pointer(x)
 *          if is_not_marked(x)
 *             marke(x)
 *          for_each_fields(x)
 *            DFS(x->f)
 *            
 *   mark
 *    for each root_node 
 *       DFS(root_node)
 *    
 *  sweep
 *    p = heap_head
 *    while p < heap_end
 *      if is_marked(p)
 *           clean_mark(p)
 *      else
 *         add_free_list(p)
 *         p += sizeof(*p)
 *    
 *            
 *    任何为标记的节点都一定是垃圾，通关从第一个地址到最后一个地址
 *     对整个heap进行清扫, 所有的garbage 使用链表连接在一起，
 *     同时，清除所有已标记的节点的标记,为下次GC做准备
 *
 *             
 *  开销
 *    与节点的个数成正比，（和可达变量的数量成正比)
 *    清扫阶段与整个堆的大小成正比
 *             
 *  
 *  * 引用计数
 *     引用计数为0时放到空闲表中 
 *     
 *     两个重大问题：
 *        无法回收哪些构成环的垃圾
 *        增加引用计数操作的代价非常大，对于每条变更代码都需要检查
 *        
 *  * 复制式收集 copying gc
 *     堆中的可达部分是一个有向图（Why）
 *     堆中的记录是图中的节点，指针是图中的边
 *     每个程序变量是图中的一个根
 *     
 *     把所有的可达活复制到一个新的堆中（连续的，没有碎片)
 *     所有的根变为指向新的堆中, 销毁所有的旧的heap
 *     
 *     没有碎片问题
 *      复制GC的基本操作是传递(forwarding) 指针，即从old-heap 到new-heap
 *     
 *   开销
 *     与标记的节点数成正比
 *       
 *       
 *  * 分代GC
 *       基于统计中的一个普遍现象. 新创建的对象可能很快便死去,但是经过多次
 *       GC后的仍可见的对象则可能还会继续可见.
 *       因此GC应当将他的注意了放在哪些较年轻的数据上.
 *       
 *       将heap 分成若干代，
 *       每次分别对每一代进行GC，可以使用mark-sweep or copying
 *       
 *       开销... 需要实际测试
 *       
 *  * 增量 GC 
 *     可以不中断world
 *
 * 
 * valgrind 的实现方法
 * 
 * Garbage 定义：
 *  在堆中分配并且通关任何程序变量形成的指针连都无法达到的记录称之为Garbage
 *  GC 有runtime完成
 *  
 *  让编译器保证所有活跃的记录都是可达的，并尽量减少哪些可达的但非活跃记录
 */
#include <stdlib.h>

