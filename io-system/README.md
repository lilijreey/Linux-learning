
### Qus.
EE 在一个已有内容文件的开头写入数据会覆盖原来的数据吗?
	会  more see seek.c

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
  当设备可都写时kernel向process发送信号
  具体编程方法see signalDrivenIO.c

### POSIX AIO
* Link with -lrt

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

### ioctl
    iotcl 是I/O操作的杂物箱 终端IO是ioctl最大的使用方面

////////////////////////////
	//得到页大小的三种方法
	printf("system page size:%ld\n", sysconf(_SC_PAGESIZE)) ;
	//linux provide
	printf("system page size:%d\n", getpagesize()) ;
//	PAGE_SIZE

PIPE///////////////////////////////
pipe + fork
在pipe，fork后, 父子进程都有一个pipe，
即这个pipe从原来的连个口变成的4个口，父子都有write, read，口
不管从那个write口，写进数据，两个read口都可以读，（看谁先读，如果前面的以读完
在读就会阻塞），

2.pipe的结尾。当pipe的write端关闭后，在读pipe，返回0
3, 当read端关闭后，在调用write写则，1, 内核发送SIGPIPE，给process，2wreite返回EPIPE


