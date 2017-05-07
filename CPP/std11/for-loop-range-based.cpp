/*
 *  Description: range-base for loop
 */

#include <stdio.h>
#include <vector>
#include <list>
#include <map>
#include <algorithm>

// Syntax
//for ( range_declaration : range_expression) loop_statement 		
//对数组类型，会从第一个元素到最后一个，
//对用stl容器，会调用begin,end来去的范围
//对于其他Class类型， 要求有begin和end方法，返回一个支持
//*,++, != 三种操作的类型。用于loop
//
//TRAP !!!   在使用for-range时，一定不能在loop中删除容器的元素。
//           因为如果容器时map，set等。 for-range 在被编译器解释后还是会
//           翻译成for (auto it=C.begin(); it != c.end(); ++it)
//           在map， set，删除时要现++后删除，但是for-range 不会。所以会出问题
int main() {

#if 0
    int array[5] = {1,2,3,4,5};
//    for (int& x : array) {
//	x *= 2;
//	printf ( "%d\n", x );
//    }

    // 自动推倒，推出来的是引用类型
    for (auto x : array) {
	x *= 2;
	printf ( "%d\n", x );
    }
#endif
    
    //Qus. for 到底被编译器转换成什么语句？？？

    /*EE !!!
      for (auto x : array )
         dothing;
       
       the dothing can not delete  element or add element of the array
     */

    //EE 效率，和for_each 比较 比for_each, for 都快一些， 
    //优先使用range-bash for
    const int NUM=10000000;
    std::vector<int> vec(NUM, 0);
    //        -O        -O2
    //real    0m0.057s 
    //user    0m0.024s
    //sys     0m0.032s
    //for_each(vec.begin(), vec.end(), [] (int &x) { x+=3;});
    
    //real    0m0.047s
    //user    0m0.004s
    //sys     0m0.040s
    for (auto x: vec) x += 3;

    //real    0m0.059s
    //user    0m0.016s
    //sys     0m0.040s
//    for (auto it=vec.begin(); it != vec.end(); ++it)
//	*it +=3;
    //std::list<int> li(NUM, 0);
    //        -O        -O2
    //real    0m0.796s
    //user    0m0.668s
    //sys     0m0.116s
    //for_each(li.begin(), li.end(), [] (int &x) { x+=3;});
    
    //real    0m0.761s
    //user    0m0.632s
    //sys     0m0.124s
    //for (auto x : li) x += 3;
    
    //real    0m0.788s
    //user    0m0.664s
    //sys     0m0.116s
//    for (auto it=li.begin(); it != li.end(); ++it)
//	*it +=3;

    std::map<int, std::string> mm = {{1,"hello"}, {2, "bbb"}, {3, "ccc"}};
    //EE v 只能是map的valueType，不能是其他的
    for (v : mm) {
	printf ( "first:%u, sec:%s\n", v.first, v.second.c_str() );
    }

    return 0 ;
}
