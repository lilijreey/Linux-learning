
### Qus.
EE 在一个已有内容文件的开头写入数据会覆盖原来的数据吗?
	会  more see seek.c
 调用一个write 写数据到文件，会收到flush 的影响吗， flush 的默认刷新时间
 是30sec，这个到底控制哪些数据的回写

### 系统IO内核参数 
 linux kernel 使用 内存缓存磁盘的数据， 
 page cache kernel 会把会缓存读取磁盘的内容，直到用完内存，然后在把不需要的
 page cache释放掉
 kernel 的 flusher线程 把数据从dirty_buf 写入磁盘, 当达到回写的门槛是flusher
 线程就会被唤醒，直到
 ，下面的一些参数控制内存cache如何回写到内存
+  /proc/sys/vm/nr_pdflush_threads
    查看 pdflush 进程的数量，只读，
+  /proc/sys/vm/dirty_ratio
     这个参数控制文件系统的文件系统写缓冲区的大小,
     单位是百分比,表示系统内存的百分比,
     表示当写缓冲使用到系统内存多少的时候,
     开始向磁盘写出数 据.增大之会使用更多系统内存
     用于磁盘写缓冲,也可以极大提高系统的写性能.但是,
     当你需要持续、恒定的写入场合时,应该降低其数值,
     一般启动上缺省是 10.下面是增大的方法： 
     echo ’40′> /proc/sys/vm/dirty_ratio


+   /proc/sys/vm/dirty_writeback_centisecs
    这个参数控制内核的脏数据刷新进程pdflush的运行间隔.单位是 1/100秒.
    缺省数值是500,也就是 5 秒.如果你的系统是持续地写入动作,那么实际上还是
    降低这个数值比较好,这样可以把尖峰的写操作削平成多次写操作.设置方法如下： 
    echo ’200′ > /proc/sys/vm/dirty_writeback_centisecs 
    如果你的系统是短期地尖峰式的写操作,并且写入数据不大（几十M/次）
    且内存有比较多富裕,那么应该增大此数值： 

+   /proc/sys/vm/dirty_expire_centisecs
   这个参数声明Linux内核写缓冲区里面的数据多“旧”了之后,
   pdflush进程就开始考虑写到磁盘中去.单位是 1/100秒.
   缺省是 30000,也就是 30 秒的数据就算旧了,将会刷新磁盘.
   对于特别重载的写操作来说,这个值适当缩小也是好的,
   但也不能缩小太多,因为缩小太多也会导致IO提高太快.
   建议设置为 1500,也就是15秒算旧.


### 绕过IO cache
可以在open的时候 
传入 O_DIRECT 来不使用io cacahe,直接把数据写入硬盘, 这个是linux特有的

### Block 内核IO读写的单位，
   一般是1024，2048,4096,8192
   Qus. 如何查看当前文件系统的block 大小?
     getconf PAGESIZE,
     stat
   在IO操作中注意block的大小对提高IO读写性能很关键，
   最后是block的倍数,或是block对齐的

     所有在磁盘上的数据都是块大小对齐的

### 进程打开文件 文件共享
+   内核使用3个结构记录一个打开的文件
    1.  进程自身的描述符表中记录这当前打开的所有文件 
            |ft|ptr|  ptr 指向一个文件表项的指针, (不同的ptr可以指向同一个文件表项，
            例如用dup函数复制文件描述符，得到一个新的文件描述符，但是它们指向同一个文件
            表项，因此它们也指向同一个V节点表指向内核的文件表)
    2.  内核文件表，内核为所有打开的文件维持一张表。
        同一文件可以有多个表项（多个进程打开同一文件).每个表项保存
        |文件状态|文件偏移量|i/v节点指针(Linux/Unix)|
        文件状态(block append ..）可是使用fcntl 获得。 
            
    3.  i/v节点(Linux/Unix)
+   两个进程打开同一个file。 1. 2不同，3相同
+   ft的复制（dup），重定向. 1不同， 2.3 相同
+   fork 后 父子进程同一个i  1不同(数值相同)， 2.3 相同

### 同步异步  I/O asynchronous  synchronous  
    同步写分两中 synchronous 把数据写入内核buf
                 synchronized 把数据写入磁盘

* 同步 write   
            1  把数据从应用层全部写到内核buf后返回 默认行为
            2  把数据写入磁盘返回 使用 O_SYNC open
            写的过程中进程会被作为_不可中断_挂起, 从而阻塞执行

       read  1所有需要的数据全部写到应用层buf后返回，
                这段过程中进程会被挂起， 从而阻塞执行

* 异步 write  
              3. 在写操作被加入到内核写队列后返回, 操作后数据会被写入内核buf
              4. 在写操作被加入到内核写队列后返回, 操作后数据会被写入磁盘

       read   2 在读操作被加入内核都取队列后返回， 操作后返回新数据

### Signal-Driven I/O
  异步IO的一种
  当设备可能可以(Possible)读写时kernel向process发送信号
  具体编程方法see signalDrivenIO.c

### POSIX AIO
* 两个版本的AIO 
    glibc 使用多线程来模拟异步IO, 不推荐Glibc的AIO
    kernel-vative 内核级别提供的异步通知 2.6.22+

* 应用场景
  同时发起多个io操作，

* aiocb   AIO control block
  linux 上的端口完成
   每个IO操作的标示上下文
   这个结构包含了有关传输的所有信息，包括为数据准备的用户缓冲区。
   在产生 I/O （称为完成）通知时，aiocb 结构就被用来惟一标识所完成的 I/O 操作


* aio-read(struct aiocb * aiocbp ) -> 提交read操作
* aio-write 提交write操作
* aio-error(struct aiocb * aiocbp ) -> int
*   用来确定一个aiocb 的状态
*     EINPROGRESS 说明请求尚未完成
*     ECANCELLED  说明请求尚被取消
*     -1  error occued

* aio-return(struct aiocb * aiocbp ) -> ssize-t
   的到提交动作的返回结果， 和read, write 的返回值相同
   因为在异步IO中必须显示取得结果
   **必须在 aio-error/1 返回请求已经完成后才能调用**

* aio-suspend
* aio-cancel
* lio-listo

* aiocb 
  `c 
    struct aiocb {
        int aio_fildes; //file descripor
        int aio_liio_opcdoe; //valid only for lio_listio
        volatile void *aio_buf; //data buf
        size_t aio_nbytes; data buf size ??
        struct sigevent aio_sigevent; // notification structure
    }
  `

### I node V node
+   I节点：Linux 没有v Node 把 V 节点分为 I节点和目录项。I Node记录出文件名外，
        所有的重要信息, 可以使用stat 察看一个文件的I节点信息
+   目录项: 用来通过名字索引I节点，每个dir都有自己的目录项


### 文件链接
+   hard ln 自己的I节点
+   soft ln 没有自己的I节点只有文件名		

### open flags
`O_APPEND 附加
 O_CREAT  如果文件不存在则创建
 O_EXCL   常和O_CREAT 一起使用用 如果文件存在则出错
 O_TRUNC  截断文件
 O_NOCTTY 如果打开的file是终端设备， 则不将此设备分配为此进程的控制终端
 O_SYNC   每次write时都等到数据写到磁盘才返回
 O_NONBLOCK  非阻塞模式打开
 O_CLOEXEC   在进程执行exec* 函数后close fd, 默认情况下exec 后不close原有的fd
              也可使用 fcntl(FD_CLOEXEC) 设置

### fcntl
  获得已经打开的fd的属性
  改变已经打开的fd的属性
  dup fd
  为fd上锁

* 使用O_NOATIME 来指示fd不更新atime
  因为有时更新atime的消耗很大比如pipe, 但是pipe的atime对我们没有意义

### ioctl
    iotcl 是I/O操作的杂物箱 终端IO是ioctl最大的使用方面


### //得到页大小的三种方法
	printf("system page size:%ld\n", sysconf(_SC_PAGESIZE)) ;
	//linux provide
	printf("system page size:%d\n", getpagesize()) ;
//	PAGE_SIZE


### PIPE
pipe + fork
在pipe，fork后, 父子进程都有一个pipe，
即这个pipe从原来的连个口变成的4个口，父子都有write, read，口
不管从那个write口，写进数据，两个read口都可以读，（看谁先读，如果前面的以读完
在读就会阻塞），

2.pipe的结尾。当pipe的write端关闭后，在读pipe，返回0
3, 当read端关闭后，在调用write写则，1, 内核发送SIGPIPE，给process，2wreite返回EPIPE


#### readv writev  preadv pwritev
读写多个buffer到一个io流中
比一般的read, write(线性I/O) 的优点
    1. 效率高， 单个向量IO能替代多个线性IO
    2. 性能好，减少了系统调用次数
    3. 原子性

*  include <sys/uio.h>
* struct iovec {
     void *iov_base; //指向buf
     size_t iov_len; //iov_base 指向buf的长度， 在
    }
*  readv(int fd, const struct iovec iov[], int count) -> ssize_t
    从fd中读取count 个iov 数据到iov 中
    readv 会从iov[0] 一直到iov[conut-1]


### read 
read 相当与其他操作是一个很慢的操作，可有可能导致进程挂起(???)
   所有在read执行期间有可能收到一个信号，打断read,

   一个读取全部数据的操作
   ```c
   ssize_t ret;
   while (len !=0 && (ret = read(fd, buf, len)) !=0) {
       if (ret == -1) {
           if(errno == EINTR) continue;
           perror("read");
       }
       len -= ret;
       buf += buf;
   }
   ```

   nonblocking reads
   如果是读取一个设置了nonblocking fd 则除了EINTR
   ,在没有数据是还会返回-1和 EAGAIN,
   ```c
   char buf[BUFSIZE];
   ssize_t nr;
   start:
   nr = read(fd, buf, BUFSIZE);
   if (nr == -1) {
       if(errno == EINTR)
           goto start;
        if(errno == EAGAIN) //or EWOULDBLOCK
            /* resubmit later */
        else
            /* error */
   }
   ```
## close/1
   关闭一个文件，并不会影响这个文件的数据写入物理磁盘。也就是说，
   关闭一个文件，OS并不会把这个文件回写物理磁盘，

## IO 预读
kernel 层面提供了3个函数来，提供预读IO
kernel会在一次IO读取请求的时候预先读取其他的信息,
内核只对判定为是顺序读的读取才会采用预读取，对于随机读不会发送预读
从而加快下次读取时的速度
posix_fadvise(2)
readahead(2) linux specifiy

+ 设置磁盘的最大预读写大小
blockdev -setra 1024 /dev/sda 
