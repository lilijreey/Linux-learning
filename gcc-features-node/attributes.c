/*
 * =====================================================================================
 *
 *       Filename:  attempted.s
 *
 *    Description:  gcc 提供的属性测试于笔记
 *					详细信息请参考GCC Manual
 *
 *        Created:  12/26/2011 11:06:59 PM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */
#include	<stdio.h>
#include <stdlib.h>
//#include	<sys/cdefs.h>

//__attribute__ 关键字用于为函数或变量声明附属性值
///1
/* format(archetype,  string-index,  first-to-check) 
 * 检测不定参数的类型，根据printf的转移字符
 * archetype is printf, scanf, strftime, or strfmon
 * string-index 指定 那个参数是string（pattern），
 * first-to-check
    bool debug(const char * pattern, ...) const __attribute__((format(printf,1,2)));
    bool errorm(const char * pattern, ...) const __attribute__((format(printf,1,2)));
     1 是说明第一个参数是fmt， 
     2 从第二个参数开始会根据printf() 函数的格式检测参数

 */
///2
//用-pedantic 编译时，会有一些gcc的扩展警告 __extension__ 
//放在一个出现警告的前面屏蔽此警
__extension__
typedef unsigned long long int QWORD;

///3
// packed 
#if 0
	struct aa {
		char a;
		int b  ;
		char a1;
		double c;  // packed 用于 element时，该element紧挨在上一元素
	}__attribute__((packed)); // 没有空隙，用于socket

	int main() {
		printf("aa size:%.12s\n", 
				"AAA1_%08x.%08x.%08x.%08x.%08x.%n");

		return 0 ;
	}
#endif /* }}} */

///4
// noreturn
//e.g void fatal(int i) __attribute__((noreturn)) ;


///EE 5
// __warning__
//当指定__warning__ 属性时，编译或使用该函数会得到compiler的警告
//void xx() __attribute__ ((__warning__("warning msg")));
//void xx(){
//	printf("XX func\n");
//}

///EE 6
// __error__ 
//当指定__error__ 属性时，编译或使用该函数会得到compiler的警告
//void xxe() __attribute__ ((__error__("error msg")));
//void xxe(){
//	printf("XXE func\n");
//}
//
///7 EE  visibility 
//控制符号的可视性
// visibility_tpe : default, hidden, internal, protected
//hidden，常用与shared-lib，hidden的符号只能在shared-lib 中可见，对于外部不可见
//  he static 不相同。 static并不影响symbol的Vsi属性
void 
__attribute__ ((visibility("hidden")))
hidden() {
    printf("hidden\n");
}
// 

//EE 8 const, pure
//指明一个函数不会改变任何值，可以优化掉空函数，即使不用-fipa-pure-const
/* e.g
 * void ok() {} 如果不指明const or pure 则生成的代码会有对ok的调用
 * ok() // no call ok
 * 如果指定__attribute__(pure, const) 这生成的代码不会产生对ok的调用
 * (即使没有-fipa-pure-const 优化选项)
 * 注意：这个对指向ok的函数指针不能优化！
 * void (*pok)() = ok ;
 * pok() ; genrat call instruction */

//EE 9
//声明变量又可能内有被使用，及不产生unused wraining 
//__attribute__((unused))  int i  = 3;
//  printf ( "i:%d\n", i );
    
//EE 10
// __artificial__((__hot__))
// __artificial__((__cold__))

//int main() {
//	xx();
//	xxe();
//	return 0;
//}
//
//EE __attribute__((warn_unused_result))
//   强制检测一个函数的返回值
//   有些时候，检查一个函数的返回值至关重要，如果我们
//   e.g. read syscall
#define __MUST_CHECK_RESULT __attribute__((warn_unused_result))

//EE 11 __attribute__((deprecated))
//    指出一个fun已经废弃不应在使用
//   要在用户没有检测返回值时提醒用户可以好似哦你给

// EE __attribute__((cleanup(func))
// void func(void *)
// 
// 在变量离开作用与时执行一个函数, （销毁时)
// 有点像C++的 local destroy
#define auto_free __attribute__((cleanup(auto_free_handl)))

// 注意回调函数的原型， 参数是离开作用与的变量的地址
void auto_free_handl(void * mem) {
    void **pmem = (void **)mem;
    free(*pmem);
    printf("auto_free mem:%p\n", *pmem);
}

void test() {
    {
    auto_free int* i = malloc(sizeof (int));
    printf("i addr:%p\n", i);
    *i=3;
}
    printf("leave block");
}

int main() {
    test();
    return 0;
}

