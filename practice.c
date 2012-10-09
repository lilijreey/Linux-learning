/*
 * =====================================================================================
 *
 *       Filename:  practice.c
 *
 *    Description:  study some system call 
 *
 *        Version:  1.0
 *        Created:  03/10/2011 10:11:50 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<stdio.h>

#if 0
#define	_GNU_SOURCE			/*the get_current_dir_name required this*/
#include	<stdio.h>
#include	<unistd.h>
#include	<stdlib.h>
#include	<errno.h>

int main()
{
	//get current dir pathname : 
	//char *get_current_dir_name(void) ;
	
	char *pathname_buf = NULL ;	
	printf("This current dirtory path name is:%s\n", pathname_buf =  get_current_dir_name()) ;

	free(pathname_buf) ;

	//set a new dir to /user/bin
	if(0 !=	chdir("/usr/bin") )
		perror("chdir") ;

//the x is success valure of that then the y function success,
// y must is a function name ps is pararma list

	char absolutely_pathname[50] = {0} ;

	if(NULL == getcwd(absolutely_pathname, sizeof(absolutely_pathname)))  
		perror("getcwd()") ;
	printf("The new dir is:%s\n", absolutely_pathname) ;

/* jj 
	|___aefjos_|_fejsojf
	|		   |_jfeisj
	|		   |_fejsofj
*/
	return 0;
}

#endif

#if 0
int main()
{
	char ca= 'a' ;
	char *st = "fesfsf" ;
	printf("%*c", 5, ca) ;
	printf("%*s", 6, "") ;
	printf("fesf") ;

	return 0;
}

#endif

#if 0
#include	<sys/types.h>
#include	<dirent.h>
#include	<sys/stat.h>

///test the read ./ is ok

int main()
{
	const char *dir = "./" ;
	DIR *dir_stream = NULL ;
	struct dirent *item = NULL ;

	//open ./
	if(NULL == (dir_stream = opendir(dir)))
		perror("opendir()") ;
	
	printf("%s\n", dir) ;
	//show each items
	while(item = readdir(dir_stream))
	{
		printf("%15s", item->d_name) ;
		struct stat item_stat ;
int a =		lstat(item->d_name, &item_stat) ;
printf("a=%d\n", a) ;
		if(S_ISDIR(item_stat.st_mode))
				printf("  DIR\n") ;
		else
			printf("  not dir\n") ;
	}

	closedir(dir_stream) ;

	return 0 ;
}
#endif 



//use of environment variables
//**environ was defined <stdio.h>
#if 0
#include <stdio.h>
#include <time.h>
#endif

//get user information
#if 0

#include	<stdio.h>
#include	<sys/types.h>
#include	<unistd.h>
#include	<pwd.h)>

int main()
{
	uid_t uid ;
	uid = getuid() ;
	char *login_name = getlogin() ; 
	printf("The exection user log is:%s\n", login_name) ;
	printf("UDI:%ud\n", getuid()) ;		

	sjuct passw)d *pass_ptr  ;	
	p= getpwuid(uid) ;
	pritf("name=%s,uid=%ud,gid=%ud,home=%s,gecos=%s,shell=%s\n",
			pass_ptr->pw_name, pass_ptr->pw_uid, pass_ptr->pw_gid, pass_ptr->pw_dir, pass_ptr->pw_gecos, pass_ptr->pw_shell) ;
	rfesf0 ;
	{}
}	
#endif


//test the macro  !!! QU
#if 0
#define _FUNCTION "funcName"
#define _LINE	10

#include <stdarg.h>
//怎样写一个带不定参数的宏函数
//注意1 fmt... 必须带...要不然就不能给dbg传递不定参数， 编译器会认为宏dbg（）是定参数的，
//2dbg()的定义不能带... 原因很简单你会在printf()最后加...吗？
#define dbg(log_level, fmt...) kickstart(log_level, _FUNCTION, _LINE, fmt)
void
kickstart(int log_level, const char *func_name, int line_number, const char *format, ...) 
{
	;
}	


int main() {
	kickstart(1, "hello", 2, "wourld" ) ; //不能放在全局，为什么？

dbg(2, "fmt", 1, 3) ;
	return 0 ;
}
#endif

//about const 
#if 0
int main()
{
	char str[] = "string" ;
	const char *name = "zhaoli" ;

	//name[1] = 'a' ; //error name buffer is const 
	name = str ; //ok
	//name[1] = 'a' ; //error name buffer is const 

	const char * const const_name = "double const" ;

	//const_name = str ; error the const name is a const variable
	
	int arr[2] = {1, 2} ;
	int arr1[2] = {3, 4} ;
	int *arr2 =0  ;
	arr = arr2 ; //error type int[2] from

	return 0 ;
}
#endif


/* gao tong arry and enumeration */
#if 0
enum  PBM_STORE{
	SM, 
	LO, 
	EX, 
	NONE, 
	MAX_SIZE //
}
int main() 
{
	int arry[MAX_SIZE] = {0} ;
	int bpm_arg ;
	
	switch(arg) {
		case SM:
			arry[SM] = true ;
			break;
		case LO:
			arry[LO] = true ;
			break;
		case EX:
			arry[EX] = true ;
			break;
		case NONE:
			array[NONE] = true ;
		default:
			array[MAX_SIZE] = true;
			break;
	}
	if(arry[MAX_SIZE])
		fprintf(stderr, "vailed argument") ;
}

#endif

#if 0
//gdb
typedef struct {
	char data[4096] ;
	int key ;
}item ;
item arr[] = {
	{"bill", 3}, 
	{"neil", 4}, 
	{"rick", 2}, 
	{"john", 5}, 
	{"alex", 1}, 
} ;

void sort(item *a, int n) 
{
	int i = 0, j =0 ;
	int s = 1;

	for(; i<n && s != 0; i++) {
		s = 0 ;
		for(j = 0; j < n-1; j++) {
			if(a[j].key > a[j+1].key) {
				item t =a[j] ;
				a[j] = a[j+1] ;
				a[j+1] = t ;
				s++ ;
			}
		}
		n-- ;
	}
}

#include <stdio.h>

int main() 
{
	int i ;
	sort(arr, 5) ;
	for(i = 0; i < 5; i++) {
		printf("array[%d]={%s, %d}\n", i, arr[i].data, arr[i].key) ;
	}
	return 0 ;
}
#endif

#if 0
#define NDEBUG
#include <assert.h>
#include <stdio.h>
int main()
{
	int a ;
	int b ;
	a =(1>2) ;
	b =(1<2) ;
	printf("a=%d, b=%d\n", a, b ) ;
	assert(-1 > 0) ;
//	assert(0) ;
	return 0 ;
}

#endif

//system()
#if 0
#include	<stdio.h>
#include	<stdlib.h>
int main()
{
	printf("Running ps with system\n") ;
	system("ps ax") ;
	printf("Done.\n") ;
	return 0 ;
}
#endif


//fifo 多次调用open
#if 0
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main()
{
可以因为每次调用都得到指向同一个fifo的不同df	
}
#endif

//show STDIN_FILENO STDOUT_FILENO
#if 0

#include <stdio.h>
#include	<unistd.h>
//0, 1, 2
int main()
{
	printf("STDIN_FILENO=%d:STDOUT_FILENO=%d:STDERR_FiLENO=%d\n", 
			STDIN_FILENO,STDOUT_FILENO,STDERR_FILENO) ;
	return 0 ;

}
#endif

// test a specfiy machine word size
int main()
{
	unsigned char ch_size = -1 ;
	unsigned int int_size = -1 ;

	printf("the machine size of word is :%d\n", int_size / ch_size) ;
	return 0 ;
}
