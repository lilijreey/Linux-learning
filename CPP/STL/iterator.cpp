/*
 *       Filename:  iterator.cpp
 */
#include	<iostream>
#include	<vector>
#include	<string>
#include	<algorithm>
#include	<iterator>

///迭代器的类型
//  
//

///std::next(iterator, step) sine c++11  -> new iter
// 把一个指定的迭代他，++(向后移动 step 次
///std::advance -> void
// 和std::next 相同，只是next只接受 ForwardIterator 的Iter引用
// advance接受InputIterator, advance 不返回之接受引用

///std::prev(iterator, step) sine c++11
// 把一个指定的迭代他，-- step 次

///std::distance
// 返回两个迭代器的距离

using namespace std ;
int main() {
	int i = 1 ;
	vector<int> v ;
	v.push_back(i++) ;
	v.push_back(i++) ;
	v.push_back(i++) ;
	v.push_back(i++) ;
	v.push_back(i++) ;
	v.push_back(i++) ;
	v.push_back(i++) ;


	vector<int>::const_iterator cit = v.begin() ;
	for (; cit != v.end(); ++cit)
		cout << *cit << ' ' ;

	cout << "\nadvance" << endl ;
	cit = v.begin() ;
	cout << *cit << endl ;

	/*  advance three elements forward */
	advance(cit, 3) ;
	cout << *cit << endl ;

	/* step one element backward */
	advance(cit, -1) ;
	cout << *cit << endl ;

	/*
	 * reverse iterator
	 */
	
	cout << "Reverse" << endl ;
	vector<int>::const_reverse_iterator rcit(v.begin()) ;
	cout << *rcit << endl ; // the value is forward begin()


	vector<int>::const_reverse_iterator rcit2(v.end()) ;
	cout << *rcit2 << endl ; // the value is forward end()

	vector<int>::const_iterator it ;
	it = rcit.base() ;


}

