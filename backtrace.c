/**
 * @file     backtrace 相关的API.
 *           head file: <execinfo.h> libc 
 *           作用: 支持程序自己debug,打印调用栈
 *           需要使用 -rdynamic 编译, 才可以显示符号名
 *           
 * @author   lili  <lilijreey@gmail.com>
 * @date     05/23/2013 03:09:51 PM
 *
 */

// 通常在收到段错误 SIGSEGV 时使用
//
// Qus. 程序宕机时，直接产生core文件， 那么使用backtrace 和生产core文件有哪些不同的地方?
// 
// 使用addr2line 把地址转化成文件行号

///int backtrace(void **buffer, int size);
//    得到调用堆栈信息
///@p buffer [OUT], 调用堆栈会村到buffer中
///@p size [IN] 指定buffer大小
///@return 返回实际存入buffer的数量


///char **backtrace_symbols(void *const *buffer, int size);
//把地址转换成符号
///@p buffer backtrace得到的buffer
///@p size  buffer的大小
///@return 转换后的符号数组，空间由backtrace_symbols 使用malloc分配，需要调用者释放

///void backtrace_symbols_fd(void *const *buffer, int size, int fd);
//和 backtrace_symbols_fd 相同只不过把结果直接写入到fd中


#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void
myfunc3(void)
{
    int j, nptrs;
#define SIZE 100
    void *buffer[SIZE];
    char **strings;

    nptrs = backtrace(buffer, SIZE);
    printf("backtrace() returned %d addresses\n", nptrs);

    /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
       would produce similar output to the following: */

    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }

    for (j = 0; j < nptrs; j++)
        printf("%s\n", strings[j]);

    free(strings);
}

static void   /* "static" means don't export the symbol... */
myfunc2(void)
{
    myfunc3();
}

void
myfunc(int ncalls)
{
    if (ncalls > 1)
        myfunc(ncalls - 1);
    else
        myfunc2();
}

int
main(int argc, char *argv[])
{
    int callcount=3;
    myfunc(callcount);
    exit(EXIT_SUCCESS);
}
