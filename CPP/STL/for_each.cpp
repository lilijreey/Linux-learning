/*
 * =====================================================================================
 *
 *       Filename:  for_each.cpp
 *
 *    Description:  user for_each(), test the format of for_each's op
 *
 *        Version:  1.0
 *        Created:  10/15/2011 07:38:35 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<vector>
#include	<iostream>
#include	<algorithm>

class Ex {
	int a ;

public:
	Ex(int i=0) : a(i) {}
	void show() const {
		std::cout << a  << std::endl ;
	}


};

	void show(const Ex &e) {
		std::cout << "show1" << std::endl ;
		e.show() ;

	}

int main() {
	std::vector<Ex> ee ;

	ee.push_back(1) ;
	ee.push_back(2) ;
	ee.push_back(3) ;
	ee.push_back(4) ;

	std::for_each(ee.begin(), ee.end(), 
			show) ;

	
}
