内核级别的process 

* `task_struct 进程描述符结构<linux/sched.h>`
   kernel 使用 `task_struct`  来关于进程，这个结构包含了一个进程
      的所有信息

*  进程状态 state 共有10种
   <script language=c>
   #define TASK_RUNNING		0       //	"R (running)",		/*   0 */
   #define TASK_INTERRUPTIBLE	1       //	"S (sleeping)",		/*   1 */
   #define TASK_UNINTERRUPTIBLE	2   //	"D (disk sleep)",	/*   2 */
   #define __TASK_STOPPED		4       //	"T (stopped)",		/*   4 */
   #define __TASK_TRACED		8       //	"t (tracing stop)",	/*   8 */
   #define EXIT_ZOMBIE	     	16      //	"Z (zombie)",		/*  16 */
   #define EXIT_DEAD		    32      //	"X (dead)",		/*  32 */
   #define TASK_DEAD	     	64      //	"x (dead)",		/*  64 */
   #define TASK_WAKEKILL		12      //	"K (wakekill)",		/* 128 */
   #define TASK_WAKING	     	256     //	"W (waking)",		/* 256 */
   </script>

* Linux 中所有的process都在一个树中，最顶层的process是 `init_task`
  `init_task` 自身是静态分配的, `init_task.parent 指向自己`

* 进程通过 `all_thread_info` 来分配内核栈

* `thread_info` 
   内核中对线程的表示， 定义是体系相关的
   `task_struck->stack 成员指向当前运行的线程 
     #define task_thread_info(task)	((struct thread_info *)(task)->stack)

