#include	<stdio.h>
#include	<unistd.h>
#include	<stdlib.h>

/* thses functions use to atexit register */
#if 0
void re_1() {
	printf("re_1\n") ;
}

void re_2() {
	printf("re_2\n") ;
}

void re_3() {
	printf("re_3\n") ;
}

int main () {
	atexit(re_1) ;
	atexit(re_2) ;
	atexit(re_3) ;

	return 0 ;
}
#endif

/*
 * show environ table
 * if you want to list environ-list use envirson variable
 * getenv(), setenv() ;
 */
//extern char **environ ;
//int main() {
//char **en = NULL ; 
//	for (en = environ; *en != NULL; *en++) {
//		printf("%s\n", *en) ;
//	}
//
//	return 0 ;
//}
//

//#include	<string.h>
//#include	<zhao/tools.h>
//
//NOTE: 使用environ变量需要声明一下
//fork: 后父子进程的env一样
extern char **environ ;

int main() {
    int i;
    for (i = 0; environ[i]; ++i) {
        printf("%s\n", environ[i]) ;
    }

    return 0 ;
}

