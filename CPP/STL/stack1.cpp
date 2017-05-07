/*
 * =====================================================================================
 *
 *       Filename:  stack1.cpp
 *
 *    Description:  use stack
 *
 *        Version:  1.0
 *        Created:  10/16/2011 08:46:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<stack>
#include	<iostream>

using namespace std ;

int main() {
	stack<int> st ;

	st.push(1) ;
	st.push(2) ;
	st.push(3) ;
	st.push(4) ;
	st.push(5) ;

	cout << st.top() << ' ' ;
	st.pop() ;
	cout << st.top() << ' ' ;
	st.pop() ;

	cout << endl ;

	// modify top element
	cout << st.top() << ' ' ;
	st.top() = 77 ;
	cout << st.top() << ' ' ;

	st.push(4) ;
	st.push(5) ;
	st.pop() ;

	cout << "while" << endl ;
	while (!st.empty()) {
		cout << st.top() << ' ' ;
		st.pop() ;
	}

}
