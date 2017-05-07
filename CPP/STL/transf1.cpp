/*
 * =====================================================================================
 *
 *       Filename:  transf1.cpp
 *
 *    Description:  use transform func
 *
 *        Version:  1.0
 *        Created:  10/13/2011 07:38:22 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<algorithm>
#include	<iostream>
#include	<vector>
#include	<iterator>

using namespace std ;

class Ex{
	int a ;

public:
	Ex(int i) : a(i) {}
	void show() const  {
		cout << a << ' ' ;
	}

	Ex& sub() {
		a -= 10 ;
		return *this ;
	}

};

Ex& sub2(Ex &x) {
	return x.sub();
}
int main() {
	vector<Ex> v ;
	v.push_back(1) ;
	v.push_back(2) ;
	v.push_back(3) ;
	v.push_back(4) ;
	v.push_back(5) ;

	for_each(v.begin(), v.end(), 
			mem_fun_ref(&Ex::show)) ;
	/* not write this mem_fun_ref(&Ex::show())) */


	Ex n(10) ;
	fill(v.begin(), v.end(), 
			n) ;
	for_each(v.begin(), v.end(), 
			mem_fun_ref(&Ex::show)) ;
			

	

}
