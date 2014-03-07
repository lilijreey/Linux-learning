/*
 * =====================================================================================
 *
 *       Filename:  __sync.c
 *
 *    Description:  
 *
 *        Created:  03/06/2013 10:09:44 AM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */
#include <stdlib.h>

//EE __sync_synchronize() 
//  发出一个full barrier
//  在所有的线程上同步数据
//  强制保证cpu执行的顺序
//  e.g. exp1
//       exp2
//       exp3
//       __sync_synchronize()  强制exp4 在上面三条以后执行
//       exp4
