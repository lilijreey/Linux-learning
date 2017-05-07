/*
 * =====================================================================================
 *
 *       Filename:  ioiter1.cpp
 *
 *    Description:  use stream iterators
 *
 *        Version:  1.0
 *        Created:  10/08/2011 06:32:57 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<iostream>
#include	<vector>
#include	<string>
#include	<algorithm>
#include	<iterator>
using namespace std ;

int main() {
	vector<string> coll ;
	copy(istream_iterator<string>(cin), 
	     istream_iterator<string>() , 
		 back_inserter(coll)) ;
	sort(coll.begin(), coll.end()) ;

	coll.~vector() ;

	/*
	 * Vector's iterator 
	 */

	vector<int> coll2 ;
	if (coll.size() > 1) {
		sort(++coll2.begin(), coll2.end()) ;
	}
}
