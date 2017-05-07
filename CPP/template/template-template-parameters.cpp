/*
 * =====================================================================================
 *
 *       Filename:  template-template-parameters.cpp
 *
 *    Description:  
 *
 *        Created:  12/06/2011 09:35:19 PM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */

#include	<iostream>
#include	<vector>
#include	<deque>
#include	<list>

using namespace std ;
// type1
template <typename T, 
		  template <typename X, 
		            typename Alloc> class CONT >
class C1{
public:
	CONT<T, std::allocator<T> > elems ; // use std::allocator
	void show(); 
};

template <typename T, 
		  template <typename X, 
		            typename Alloc> class CONT >
void C1<T, CONT>::show() {
	std::cout << "c1" << std::endl ;
}
 

// type2
// 第二个参赛就是声明他的类型，就像函数原型一样
template <typename T, 
		  template <typename X, 
		            typename = std::allocator<X> > class CONT >
class C2 {
public:
	CONT<T> elems ; // not use allocator beacuse two parameter is default std::allocator 
	void show() { 
		std::cout << "c2" << std::endl ;
	}
		
};

int main() {
	C1<int, std::vector> c1 ;
	c1.show() ;

	C2<char, std::list> c2 ;
	c2.show() ;

}


