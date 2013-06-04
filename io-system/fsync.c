/**
 * @file     fsync.c 
 *          文件同步写入到储存器 相关函数
 *          fdatasync.
 *          要保证已经写入file的数据真正的写到的磁盘上。
 *          使用默认的 write时不能这样保证的。 必须使用同步IO
 *
 *  @p fsync/1(fd)
 *      fsync/1 会阻塞直到对fd的修改已经写到磁盘上
 *      fsync/1 除了同步file的内容，还会同步file的元数据，如size, st_time
 *      所以fsync/1 要写两个地方，metadata 和文件内容并不再同一地发
 *      如果不需要更新元数据可以使用 fdatasync/1
 *      
 *  @p msync/3
 *      如果使用map 映射文件，可以使用msync/3 来同步
 *      
 *  @p fdatasync/1(fd)
 *     和fsync/1 类似，但是不到必须的时刻不会更戏 metadata
 *     
 *  @p sync_file_range/4
 *     有选择的同步file中的部分内容
 *  @note 如果磁盘有自己的缓存，则fsync,fdatasync 等函数不能保证数据
 *         已经写入物理戒指，当这些函数返回时.
 *         
 *  @note open(2) 的参数 @p O_SYNC 的语义和fsync 相同.
 *      POSIX 定义了三个 同步IO 标记 @p O_DSYNC @p O_RSYNC
 *      但是linux之实现了 O_SYNC, 这三个标记当前语义相同
 *      都是 O_SYNC.
 *
 *         
 *  由于同步IO的速度很慢，而且 fdatasync 不见得一定不更新metedata
 *     所以对于 log， 一般固定大小，这样fdatasync 时就不用写metadata，
 *     会快一些
 *
 * @p sync/0 syncfs/1
 *    把所有的buf中的更改，写到底层的文件系统，(把写磁盘的请求写入请求队列)
 *    并不是写入磁盘
 *    
 * @author   lili  <lilijreey@gmail.com>
 * @date     05/13/2013 10:00:58 PM
 *
 */

#include <stdlib.h>

