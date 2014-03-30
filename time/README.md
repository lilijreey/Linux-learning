# Linux 对时间的定义

### 参考书籍
+   Linux内核设计与实现
+   Linux系统编程
+   Unix环境高级编程

### 概念
*  时区
*  时差
*  GMT 格林尼治时间
    0时区，时间的原点
    根据地球旋转来计算
*  UTC 世界协调时间
    使用原子钟来计算
   UTC 等于GMT +0 时区；w
*  DST
   夏令时
* 格里高历 公历

### 内核
+   大量内核函数是时间驱动的

### 编译链接
如果使用了<time.h>文件编译是需要链接 lrt库。是glib
中支持reaytime。在原文件之后链接 e.g. file -lrt

### 编程实践
    注意在程序中不要大量使用取得系统时间的函数，有可能会导致大量的系统调用。
    应该定时更新时间比10ms一次。 其他用到时间的都使用上次得到的时间缓存
`    for (int i=0; i<=10000000; ++i) { // 1KW
        //time 的调用代价非常小,没有系统调用
        //real	0m0.041s
        //user	0m0.040s
        //sys	0m0.000s
        time(NULL);
        
        // gettimeofday 的代价比time大很多,没有系统调用
        //real	0m0.219s
        //user	0m0.216s
        //sys	0m0.000s
        //gettimeofday(&val, NULL);


### Linux内核从三个角度来度量时间：
> 1. 真实时间（生活中的表上的时间）
> 2. 进程时间（一个进程消耗的时间,包括内核时间和用户时间, time 命令中的real) 
>> 其实time是一个shell _key-word_，还有一个time程序_/usr/bin/time_ 他的输出比time要强大.
>> QU: 为啥执行time的时候real比sys+usr大?  ANW:因为有可能有大量的io等待.  
> 3. 单调时间（系统启动到现在的时间）。这个时间是递增.单调时间的重要
  性不在于当前值而在于可以保证时间源的线性增长，用来计算两次时间的差值.

# Linux时间的表示
> 1. 相对时间
> 2. 绝对时间
  Unix 使用从大纪元（1970-1-1-00:00:00)以来经过的秒数表示绝对时间.

# Linux 时间的实现
### 软件时钟.
是由内核维护的时钟，linux通过软件时钟来追踪时间进程。  

### 系统计时器(system timer).
由内核初始化的一个有特定频率的时钟.一个可编程的硬件一固定间隔
发送时间中断(timer interrupt).
时间系统的基础，系统以他来计算系统时间，墙上时间(well time)
X86上主要的系统计时器是PIT（ke编程中断计时器),
其他的时间源还包括TSC和localAPIC timer

### tick(jiffy) 系统滴答
当系统计时器一个周期结束时，内核将jiffy增加一个单位。

### jiffies 计数器
一个全局变量，记录系统从开机到现在的tick数量.2.6内核以前是32bit的，从2.6开始使用64bit.

### HZ The Tick Rate
+  Linux 系统中系统计数器的频率.直接影响到**系统的最小时间精度**.
+  HZ 的值系统相关的。不能期望他等于某个值。历史上X86 HZ=100, 表示系统计时器
的周期是0.01秒。也就是1秒钟jiff增加100. HZ在编译内核时是可选的X86下有 100，250 1000
+   _运行时得到当前HZ_ posix定义了`sysconf(_SC_CLK_TCK)` see get-sys-HZ.c
+   HZ实在编译内核是指定的.
+   HZ大的好处：定时更加精确， poll() and select()
    不好：增加了处理中断的时间

### 时间的数据结构
+   `time_t`
    define in <time.h> header file.
    表示自大纪元来了过去的秒数. 在到多数Unix上是long

+   timval 
define in <sys/time.h> header file.
提供了毫秒级的精度10-3. 
      `struct timeval {  
        time_t tv-sec; /*seconds*/  
        suseconds_t tv_usec; /*microseconds*/  
      }`  

+   timespec     
    define in _<time.h>_ header file.
    提供了纳秒级的精度10-9. 
        `struct timespec {  
          time_t tv_sec; /*seconds*/  
          long tv_nsec; /*nanoseconds*/  
        }`  
 
+ struct itimerspec
    define in <sys/time.h> header file.
    用于poisx timer_settime() 的使用，指定首次定时的时间，和重启后定时的时间 纳秒级精度
    {
        // 本次定时的时间, 当自动重启时，it_value会被赋值为it_interval 的值
        struct timespec it_value; 
        // 重启时定时的时间,if is 0，不自动重启定时器; 
        struct timespec it_interval;
    }
    
+ struct itimerval delay;
    define in <time.h> header file.
    用于 setitimer() 和 struct itimerspec 作用相同，只不过时间精度是毫秒级别的10^-6
    {
        // 本次定时的时间, 当自动重启时，it_value会被赋值为it_interval 的值
        struct timeval it_value; 
        // 重启时定时的时间,if 为0，不自动重启定时器; 
        struct timeval it_interval;
    }

### POSIX 时钟
POSIX 的timespec 可达到纳秒，如果一个机器的HZ为100，可以准时返回吗，
    还是在10000纳秒时返回.看实现，如果使用不同的时间源(X86下是TSC)就和HZ无关
    POSXI标准中的函数一般都以clock_开头 编译时链接 -lrt


### 函数综述
    总体上看对一个操作Linux都提供3个相同功能的函数，但是时间精度不同，分别是秒，微秒，纳秒

### 得到时间精度
+   得到HZ
    sysconf(_SC_CLK_TCK )
+   得到POSIX时间精度
    clock_getres

### 得到当前时间
+   time() 返回在大纪元以来的秒数
+   gettimeofday() 获得微妙级的精度
+   `clock_gettime()` posix 提供的得到不同时间源的当前时间 (推荐使用)
    +   得到大纪元墙上时间
    +   得到系统启动到现在的时间
    +   得到**当前进程**启动到现在的时间  
    +   得到**当前线程**启动到现在的时间  
+ strftime/ strptime
  格式化时间输出

## 得到进程内核态调用时间，用户态调用时间
+   times 和bash time功能相同

### 设置当前时间,和得到当前时间对应
+   stime()
+   settimeofday()
+   `clock_settime()` (推荐使用)

### 睡眠
+   sleep(sec) 返回剩余的秒数
        如果希望进程睡眠指定秒数可以 int sec =5 ;while (sec) sec = sleep(sec);
        or while ((sec = sleep(sec)));
+   usleep(usec) 睡眠多少微秒
        The usleep() function suspends execution of the calling thread.
+   select() 提供微秒级的精度
+   nanosleep(sleepTime, remainTime) poxis标准**不使用信号实现**(推荐使用)

### 定时器 
+   主要用处：定时回调 比如心跳函数的实现
+   alarm() 到时发送信号，不会自动重启动。只是定时不会使线程进入睡眠
        返回前一个alarm的剩余秒数，如果没有返回0
+   getitimer,setitimer (间歇定时器) 可以自动重启,有三种, 提供微妙级别 10^-6
       The  system  provides  each  process  with  three interval timers, each
       decrementing in a distinct time domain.	When any timer expires, a signal is sent to the process, and the timer (potentially) restarts.
see alarm.c
+   `timer_create, timer_settime'. posix func 最高级控制最强的定时器

### date
 strftime 格式化time_t 为时间


/* Return pointer to a static char[] buffer in which zero-terminated
   string-representation of TM (in form hh:mm:ss) is printed.
   If TM is NULL, the current time will be used.  */

char *
time_str (time_t t)
{
  return fmttime(t, "%H:%M:%S");
}

/* Like the above, but include the date: YYYY-MM-DD hh:mm:ss.  */
char *
datetime_str (time_t t)
{
  return fmttime(t, "%Y-%m-%d %H:%M:%S");
}
