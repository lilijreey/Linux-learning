/*
 * =====================================================================================
 *
 *       Filename:  log.c
 *
 *    Description:  syslog funcs
 *
 *        Created:  09/02/2012 08:22:01 AM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */

#include <syslog.h>
#include <stdlib.h>

/*
 * 1.所有的syslog函数都没有返回
 *
 * 2.使用syslog 输出内容到系统日至文件。第一个参数是输出的优先级,
 * 剩下的和 和printf 一样，
 * ubuntu 下会输入到 /var/log/syslog
 * 不用openlog也可输入
 * e.g. syslog(LOG_ERR | LOG_USER, "opps - %m\n")
 * the %m is conversion specifier has been replaced by an description
 * of the error, output like ... opps - Not a file
 *
 *
    3. openlog(const char *ident, int option, int facility);
       设定输出的格式,和行为
       ident 会添加到每条message前头,通常是程序名称
       facility 是级别和锁说明message的来源
       可选的，调用syslog时会自动调用openlog，如果需要的话 这时ident为NULL

    4. closelog() 可选的
    5. setlogmask(int mask); 设置输出级别，屏蔽不要的级别
       没有设置时，都是可输出的
 */

int main() 
{
    openlog(__FILE__, LOG_PID, LOG_USER );
    syslog(LOG_INFO, "evan this is msage");
    return 0;
}

