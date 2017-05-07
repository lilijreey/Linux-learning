/*
 * =====================================================================================
 *
 *       Filename:  ostriter.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/11/2011 05:51:26 AM
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
#include	<iterator>
#include	<algorithm>

using namespace std ;

int main() {
	ostream_iterator<int> wit(cout, "\n") ;

	*wit = '1' ;
	++wit ;
	*wit = 77 ;
	++wit ;
	*wit = 21 ;


	vector<int> col1 ;
	for (int i=1;i<=9; ++i) {
		col1.push_back(i) ;
	}

	copy(col1.begin(), col1.end(), 
			ostream_iterator<int>(cout)) ;
	cout << endl ;

	copy(col1.begin(), col1.end(), 
			ostream_iterator<int>(cout, ":")) ;
	cout << endl ;

}

/* Iterator Traits */
template <typename iterator>
typename std::iterator_traits<iterator>::difference_type
distance(iterator pos1, iterator pos2) {
	return distance(pos1, pos2, 
			std::iterator_traits<iterator>::iterator_category()) ;

}

template <typename rit>
typename std::iterator_traits<rit>::difference_type
distance(rit pos1, rit pos2) {
	return pos1 - pos2 ;
}



