##内核级别的process 

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

## 内核线程
* 创建内核线程
  kthread_create 必须由其他内核线程创建
    新创建的线程不会主动运行，除非现世调用 wake_up_process()

* kthread_stop 

* 删除进程描述符 task_struct;
  do_exit() 后线程状态为EXIT_ZOMBIE
  线程终止后并不会清理内核堆栈 thread_info, task_struct 结构，会保留他们
  以被父进程处理。 所以只有在父进程wait后或明确通知内核不关心后才会
  删除task_struct. release_task()
  
## 进程调度
* O1 算法 
   在2.6.23 以前
   分公平但是对交互式应用的支持不足

* CFS 完全公平算法
  RSDL Rotating Staircase Deadline scheduler
  2.6.23 后代替了O1算法
  Linux 使用CPU的使用比来调度进程, 比如有10个进程，每个进程应该使用%10
   的CPU，但是实际中有的进程当前CPU使用率少于%10，他就会立即抢占，比他多的进程

