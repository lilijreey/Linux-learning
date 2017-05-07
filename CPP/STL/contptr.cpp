/*
 * =====================================================================================
 *
 *       Filename:  contptr.cpp
 *
 *    Description:  use contper.hpp
 *
 *        Version:  1.0
 *        Created:  10/10/2011 07:49:59 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<iostream>
#include	<list>
#include	<deque>
#include	<algorithm>
#include	"contptr.hpp"
using namespace std ;

void printCouontPtr(CountedPtr<int> elem) {
	cout << *elem << ' ' ;
}

int main() {
	static int values[] = {3, 5, 9, 1, 6, 4} ;

	// two defferent collections
	typedef CountedPtr<int> IntPtr ;
	deque<IntPtr> coll1 ;
	list<IntPtr> coll2 ;

	// Insert shared objects into the collections
	// same order in coll1 coll2
	// reverse order in coll1 coll2 
	for (int i= 0; i < sizeof(values) / sizeof(values[0]); ++i)
	{
		IntPtr ptr(new int(values[i])) ;
		coll1.push_back(ptr) ;
		coll2.push_front(ptr) ;
	}

	for_each(coll1.begin(), coll1.end(), 
			printCouontPtr) ;
	cout << endl ;

	for_each(coll2.begin(), coll2.end(), 
			printCouontPtr) ;
	cout << endl ;

	// Modify values at different places
	*coll1[2] *= *coll1[2] ;
	// (**coll1.begin() get number in IntPtr
	(**coll1.begin()) *= -1 ;
	(**coll2.begin()) = 0 ;

	for_each(coll1.begin(), coll1.end(), 
			printCouontPtr) ;
	cout << endl ;

	for_each(coll2.begin(), coll2.end(), 
			printCouontPtr) ;
	cout << endl ;

}
