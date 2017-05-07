/*
 *  Description: 新的nullptr key word
 */


#include <stdio.h>
//EE 1 nullptr 指明是空指针
//其本质是一个内定的常量，是一个辨识，不是一个整数
void nn(int *) {printf ( "int *\n" );}
void nn(int) {printf ( "int\n" );}

//EE nullptr 与NULL的区别
int main() {
    //区别1
    //这一原本需要调用int*的但是调用的是int的重载版本，g++会给出警告
    nn(NULL);
    nn(nullptr);

    char *a= NULL;
    char *b= nullptr;
    //NULL 和 nullptr 在数值上相等
    if (a == b) 
	printf ( "a == b\n" );
    else
	printf ( "a != b\n" );

    //EE nullptr 和interger的区别，nullptr 不是int类型，无发比较
//    int zero = 0;
//    if (nullptr == zero) //error 一个无效的比较
//	printf ( "nullptr eq 0\n" );

    //EE nullptr 和常量0的比较，与0相等
    if (nullptr == 0)
	printf ( "nullptr == 0\n" );



    
	return 0 ;
}


