/**
 * @file     sendfile.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     04/09/2014 11:58:00 AM
 *
 */
#include <stdlib.h>
#include <sys/sendfile.h>

// 内核直接把一个文件中的内容send到socket上，而不需要在通过应用层读取
// 在发送，效率很高
//
//
// Qus. sendfile 是同步的吗？ 就是发送完了才返回
//
//sendfile 不会改变in_fd 的current offset
//
//sendfile(out_fd, in_fd, off_t*[OUT], count)
// 如果 off_t 为NULL，就从in_fd 的当前offset 读取,并且会改变in_fd 的当前offset
// 如果 off_t 不为NULL， 就从制定的offset读取， 并且不会改变in_fd 的当前offset
// 
//  off 用来传递开始读取的offset，并且会返回最新的offset
// RETURN 返回发送的字节数字
