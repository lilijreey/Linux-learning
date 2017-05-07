/*
 *  Description: 
 */
#include <stdio.h>
//C++98 中class会默认生成4个函数
//但是有时不想这样，
//EE C++11 中提供了 =delete 的语法来支持
//EE 只有函数声明是可以使用 =delete, 任意函数，mem，non-mem
//EE =deault explicit的声明希望使用声明的函数,只能是mem, 
class Os {
public:
    Os() {printf ( "Os construction\n" );}
    ~Os() {printf ( "Os destruction\n" );}
    void ss() {printf ( "SS\n" );} 
    Os(const Os& o)  = delete ;
};
void x1() =delete;
//void x1() { printf ( "X1\n" );}


int main() {
    Os s1;
    s1.ss();
    //Os s2(s1);
    x1();

    
    return 0 ;
}


