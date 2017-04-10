/**
 * @file     libevent.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     03/05/2014 11:38:41 AM
 *
 */
#include <stdlib.h>

/*
 * 引擎核心是 event_base,
 * 每个event是 struct event
 *      一个event可以代表3中不同的事件， 1定时器，2信号，3socket，文件
 * 
 * 向event_base 添加一个event
 * 1. 初始化一个evnet *. event_new, malloc
 *                    *. event_assign 不分配内存只设置
 *                    *. event_set, 类似与 event_assign, 但是event_base 
 *                         使用的是默认的全局 current_base
 *                         不建议使用了
 * 2. 把event添加到 event_base
 *     event_add
 *     
 * event_free
 * event_del 只是把fd从poll模型中删除，但是并不close fd
 *                    
 * event_add 
 * 
 */
