/*
 * =====================================================================================
 *
 *       Filename:  s1.c
 *
 *    Description:  
 *
 *        Created:  10/24/2012 08:51:06 PM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */

#include <stdio.h>
void s3() ;
//__attribute__ ((visibility("hidden"))) 如果不想让外部见到shared-lib 的内部借口
//可以使用这个属性, hidden属性可以在shared-lib 内部使用

static int sss;// static 对象 Bind属性是LOCAL的;
void s1()
{
    printf("s1.c s1 function\n");
    s3();
}
