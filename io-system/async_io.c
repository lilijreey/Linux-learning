/*
 * =====================================================================================
 *
 *       Filename:  async_io.c
 *
 *    Description:  异步IO
 *
 *        Created:  04/19/2013 05:40:50 PM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */
/**
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
 *
 *
 *
 */

#include <stdlib.h>

