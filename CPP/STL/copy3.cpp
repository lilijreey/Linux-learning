/*
 * =====================================================================================
 *
 *       Filename:  copy3.cpp
 *
 *    Description:  Iterator Adapters
 *
 *        Version:  1.0
 *        Created:  10/08/2011 06:19:15 AM
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
#include	<list>
#include	<deque>
#include	<set>
#include	<algorithm>
using namespace std ;
int main() {
	list<int> colla;

	for (int i = 1; i <= 9 ; ++i)
	{
		colla.push_back(i) ;
	}
 

	vector<int> collb ;
	copy(colla.begin(), colla.end(), 
			back_inserter(collb)) ;
	for (vector<int>::const_iterator cit = collb.begin();
			cit != collb.end(); ++cit)
	{
		cout << *cit << " " ;
	}
	cout << endl ;

	cout << "deque" << endl ;
	deque<int> collc ;
	copy(colla.begin(), colla.end(), 
			front_inserter(collc)) ;
	for (deque<int>::const_iterator cit = collc.begin();
			cit != collc.end(); ++cit)
	{
		cout << *cit << " " ;
	}
	cout << endl ;

	cout << "set" << endl ;
	set<int> colld ;
	copy(colla.begin(), colla.end(), 
			inserter(colld, colld.begin())) ;
	for (set<int>::const_iterator cit = colld.begin();
			cit != colld.end(); ++cit)
	{
		cout << *cit << " " ;
	}
	cout << endl ;


}
