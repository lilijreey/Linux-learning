/**
 *    Description:  异步IO Glibc 实现的
 *
 *        Created:  04/19/2013 05:40:50 PM
 *  @note Link with -lrt 
 *
 *  @note struct aiocb (IO control block) .
 *
 *  相当于对fd的封装
 *    所有的aio操作都需要aiocb
 *     struct aiocb { //公开的
 *         int	           aio_fildes;      File descriptor 
 *         off_t	       aio_offset;      File offset 
 *         volatile void  *aio_buf;         Location of buffer 
 *         size_t	       aio_nbytes;      Length of transfer 
 *         int	           aio_reqprio;     Request priority 
 *         struct sigevent aio_sigevent;    Notification method 
 *         int	           aio_lio_opcode;  Operation to be performed;  lio_listio() only 
 * 
 *          Various implementation-internal fields not shown 
 *     };
 *
 *
 */

#include <aio.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <zhao/tools.h>


#define	AIO_BUF_SIZE 512
int main()
{
    int ret;
    ///1 使用 aio_read  需要初始化 aiocb
    struct aiocb my_aiocb;
    bzero(&my_aiocb, sizeof my_aiocb);

    // allocate data buf
    my_aiocb.aio_buf = malloc(AIO_BUF_SIZE+1);
    if (my_aiocb.aio_buf == NULL)
        perror("malloc failed");

    // initialize the necessary fields 
    // 每次读不会超过 buf 的大小
    my_aiocb.aio_nbytes = AIO_BUF_SIZE;

    int fd ;
    EV_TEST(-1, fd, open("README.md", O_RDONLY));

    my_aiocb.aio_fildes = fd;

    /// EE 设置读取的偏移量 read的时候 偏移量是在fd中维护的
    //但在异步情况下要为每次读操作都指定偏移量
    //aio_read 后kernel不会改变aio_offset 的值
    my_aiocb.aio_offset = 0;

    next:
    E_TEST(-1, aio_read(&my_aiocb));

    ///EE 使用 aio_error 来查询aiocb的状态
    //如果是 EINPROGRESS
    //就一直忙等待，直到状态发生变化为止
    while (aio_error(&my_aiocb) == EINPROGRESS) {
        printf("aio_error is EINPROGRESS\n");
    }

    if ((ret = aio_return(&my_aiocb)) > 0) {
        // read ret B data
        printf("aio_return :%d\n", ret);
        ((char*)my_aiocb.aio_buf)[ret] = '\0';
        printf("%s\n", (char*)my_aiocb.aio_buf);
        //updata aio_offset for next read 
        my_aiocb.aio_offset += ret;
        goto next;
    } else if ( ret == 0) {
        printf("aio_return :%d\n", ret);
        //read EOF
    } else {
        printf("aio_return :%d\n", ret);
        perror("aio_return failed");
        //error occued
    }

    E_TEST(-1, close(fd));
    return 0;
}
