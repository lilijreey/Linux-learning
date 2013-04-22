/*
 * =====================================================================================
 *
 *       Filename:  daemon.c
 *
 *    Description:  create a daemon process
 *                  daemon 必须是不依赖任何tty，
 *                  一般说bash是一个回话leader， 当bash结束时任何bash fork出来的
 *                  进程都有结束。 所以守护进程必须脱离tty
 *                  单实例进程，实现方式：文件锁，记录锁,程序结束时，自己创建的文件锁
 *                  会自动删除
 *
 *        Created:  01/03/2012 05:14:05 PM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */
#include	<stdio.h>
#include	<unistd.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include	<stdlib.h>
#include	<fcntl.h>
#include	<signal.h>
#include	<sys/resource.h>
#include	<zhao/tools.h>

/* 检测是否已经有本程序副本在运行 */
int already_running(void)
{
#define LOCK_FILE "/var/run/mydamon.pid"
#define LOCK_MODE 0644

    char pid[10];
    int fd = open(LOCK_FILE, O_RDWR | O_CREAT, LOCK_MODE);
    if (fd < 0) {
        //TODO syslog
        return -1;
    }

    //write pid to file
    snprintf(pid, "%d", get_pid());
    write(fd,pid, strlen(pid));

    return 0;
}

void daemonize(void) 
{
	int i;
	pid_t pid ;
	struct rlimit r1;
	struct sigaction sa ;

	/* clearn file creaton mask */
	umask(0) ;

	/* TTY can be return */
	if ((pid = fork()) < 0 ) {
		perror("frok failed") ;
        exit(1);
	} else if(pid !=0 )
		exit(0) ;

	setsid() ;

    /* System V need frok agine */
	if ((pid = fork()) < 0 ) {
		perror("frok failed") ;
        exit(1);
	} else if (pid !=0 )
		exit(0) ;

	sa.sa_handler = SIG_IGN ;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags =  0;

	E_TEST(-1, sigaction(SIGHUP, &sa, NULL)) ;

	/* set working dir */
	E_TEST(-1, chdir("/tmp")) ;

	/* close all open file */
	/* get maximum number of file descriptors */
	E_TEST(-1, getrlimit(RLIMIT_NOFILE, &r1)) ;

	if (r1.rlim_max == RLIM_INFINITY)
		r1.rlim_max = 1024 ;
	for (i=0; i<r1.rlim_max; ++i)
		close(i) ;

	/* attach file description 0, 1, 2 to /dev/null */
    /* 如果不关闭，使用printf 调用正常，但是没有显示 */
    if (open("/dev/null", O_RDWR) != STDIN_FILENO) 
        err_exit("fd is not STDIN_FILENO");
	if (STDOUT_FILENO != dup(0)) 
        err_exit("redirection stdout fd failed"); 
	if (STDERR_FILENO != dup(0)) 
        err_exit("redirection stderr fd failed");

	/* initialize the log file */
    //do something
    for (;;)
        pause();
}

int main() {
    /* one instance daemon */
 //   if (already_running()) return 0;

	daemonize() ;

	return 0 ;
}

