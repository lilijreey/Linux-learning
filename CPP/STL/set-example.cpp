/*
 * =====================================================================================
 *
 *       Filename:  set-example.cpp
 *
 *    Description:  about 6.5.4
 *
 *        Version:  1.0
 *        Created:  10/09/2011 06:14:54 AM
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
#include	<iterator>
#if 0
using namespace std ;

int main() {
	typedef set<int, greater<int> > IntSet ;

	IntSet coll1 ;

	coll1.insert(4) ;
	coll1.insert(3) ;
	coll1.insert(5) ;
	coll1.insert(1) ;
	coll1.insert(6) ;
	coll1.insert(2) ;
	coll1.insert(5) ;
	 
	IntSet::iterator pos ;
	for (pos = coll1.begin();pos!= coll1.end(); ++pos) {
		cout << *pos << ' ' ;
	}

	cout << endl ;

	pair<IntSet::iterator, bool> status = coll1.insert(4) ;
	if (status.second) {
		cout << "4 is inserted as element "
			<< distance(coll1.begin(),  status.first) + 1 
			<< endl ;
	} else {
		cout << "a already exists" << endl ;
	}

	set<int> coll2(coll1.begin(), coll1.end()) ; /* coll2 is set<int, less<> > not same with coll1 */
	for (set<int>::iterator it=coll2.begin(); it != coll2.end(); ++it) {
		cout << *it << ' ' ;
	}
	cout << endl ;

	set<int> coll3(coll1.begin(), coll1.end()) ;

	coll2.erase(coll2.begin(), coll2.find(3)) ;
	int num = coll2.erase(5) ;
	cout << num << " element(s) removed" << endl ;

	copy(coll2.begin(), coll2.end(), 
		 ostream_iterator<int>(cout, " ")) ;
	cout << endl ;



}


#endif

#include	<string>
class Ex {
	int i ;
	std::string str ;

public:
	Ex(int a=0, std::string st="") : i(a), str(st) {}
};

int main() {
	Ex e ;
}

