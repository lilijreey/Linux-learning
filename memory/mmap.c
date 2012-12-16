/*
 * =====================================================================================
 *
 *       Filename:  mmap.c
 *
 *    Description:  
 *
 *        Created:  12/30/2011 09:57:14 PM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h> //for wait
#include <sys/wait.h>

void get_page_size()
{
	//得到页大小的三种方法
    //printf("system page size:%ld\n", sysconf(_SC_PAGESIZE)) ;
	//linux provide
	//printf("system page size:%d\n", getpagesize()) ;
}

#define	BUF_SIZE	1024		/*  */

//read from mmap
void read_from_mmap(void *buf) {
    void *ptr = buf;
    for (int i=0; i <10 && i < BUF_SIZE; ++i)
     write(STDOUT_FILENO, ptr++, 1);

}

//匿名
//void  mmm

struct ok
{
    int a;
    int b;
};

int main() 
{
    void *buf;
    buf = mmap(NULL, 
                BUF_SIZE, 
                PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANON,
                -1, 0);

   if (buf == MAP_FAILED) {
        perror("mmap falied:");
        exit(1);
    }

//   struct ok * ss = (struct ok*)buf;
//   ss->a=1;
//   ss->b=2;

#if 0
   //write hello to mmap
   const char str[] = "hello";
   memcpy(buf, str, strlen(str));

   //fork
   switch (fork()) {
       case -1:
           perror("fork failed:");
           exit(1);
       case 0: //chiled 
           sleep(2);
           read_from_mmap(buf);
           break;
       default:
           wait(NULL);
           break;
   }
#endif

   //fork 后两个process都要调用 munmap ? 是的
   if (-1 == munmap(NULL, BUF_SIZE)) {
        perror("mmap falied:");
        exit(1);
    }
	return 0 ;
}

