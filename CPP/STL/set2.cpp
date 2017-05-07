/*
 * =====================================================================================
 *
 *       Filename:  set2.cpp
 *
 *    Description:  use set
 *
 *        Version:  1.0
 *        Created:  10/08/2011 08:35:39 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<iostream>
#include	<set>
using namespace std ;

int main() {
	set<int> c ;
	c.insert(1) ;
	c.insert(2) ;
//	c.insert(3) ;
	c.insert(4) ;
	c.insert(5) ;
	c.insert(6) ;

	cout << "lower_bound(3): " << *c.lower_bound(3) << endl ;
	cout << "upper_bound(3): " << *c.upper_bound(3) << endl ;

	cout << "equal_range(3): " << *c.equal_range(3).first << " "
							   << *c.equal_range(3).second << endl ;
	 if (c.lower_bound(8) == c.end())
		 cout << "c.end" << endl ;
	 else 
	cout << "lower_bound(8): " << *c.lower_bound(8) << endl ;
	 if (c.upper_bound(8) == c.end())
		 cout << "c.end" << endl ;
	 else  {
	cout << "upper_bound(8): " << *c.upper_bound(8) << endl ;

	cout << "equal_range(8): " << *c.equal_range(8).first << " "
							   << *c.equal_range(8).second << endl ;
	 }
}
