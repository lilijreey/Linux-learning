/*
 * =====================================================================================
 *
 *       Filename:  macro.c
 *
 *    Description:  GCC 提供的工具宏定义
 *
 *        Created:  12/01/2012 10:07:38 AM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */

/*
 * 1. TEMP_FAILURE_RETRY
 * head-file: ??
 * 用于sysCall 被信号打断的返回错误，再次调用sysCall
 *
 * 可能要定义 _GNU_SOURCE 才能打开 TEMP_FAILURE_RETRY 宏
 *
 *    #define TEMP_FAILURE_RETRY(expression)   
      (__extension__  
       ({ long int __result;  
           do __result = (long int)(expression);  
           while(__result == -1L&& errno == EINTR);  
           __result;})  
 *
 * e.g. int ret = TEMP_FAILURE_RETRY(recv(sock, buffer, size));
 */
