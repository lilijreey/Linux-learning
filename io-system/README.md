
### Qus.
EE 在一个已有内容文件的开头写入数据会覆盖原来的数据吗?
	会  more see seek.c


### I node V node
+   I节点：Linux 没有v Node 把 V 节点分为 I节点和目录项。I Node记录出文件名外，
        所有的重要信息, 可以使用stat 察看一个文件的I节点信息
+   目录项: 用来通过名字索引I节点，每个dir都有自己的目录项

### 文件链接
+   hard ln 自己的I节点
+   soft ln 没有自己的I节点只有文件名		


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
+   连个进程打开同一个file。 1. 2不同，3相同
+   ft的复制（dup），重定向. 1不同， 2.3 相同
+   fork 后 父子进程同一个i  1不同(数值相同)， 2.3 相同

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


