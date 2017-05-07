/*
 * =====================================================================================
 *
 *       Filename:  queuq1.cpp
 *
 *    Description:  use queue
 *
 *        Version:  1.0
 *        Created:  10/16/2011 09:28:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#include	<iostream>
#include	<queue>
using namespace std ;

int main() {
	queue<int> q ;

	q.push(1) ;
	q.push(2) ;
	q.push(3) ;
	q.push(4) ;


	cout << q.front() << endl ;
	cout << q.back() << endl ;

	q.pop() ; //delete 1
	cout << q.front() << endl ; // 2 is first
	cout << q.back() << endl ; 
}
