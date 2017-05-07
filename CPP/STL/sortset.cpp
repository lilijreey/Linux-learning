/*
 * =====================================================================================
 *
 *       Filename:  sortset.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/10/2011 07:06:19 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<iostream>
#include	<string>
#include	<algorithm>
#include	<set>
#include	<iterator>
using namespace std ;

int main() {
	set<string> co1((istream_iterator<string>(cin)),
		   (istream_iterator<string>()));

	copy(co1.begin(), co1.end(), 
			ostream_iterator<string>(cout, "\n"));
}
