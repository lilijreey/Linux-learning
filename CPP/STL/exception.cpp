/*
 * =====================================================================================
 *
 *       Filename:  exception.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/07/2011 07:46:20 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<iostream>
using std::cout ;
using std::endl ;

template <typename T>
class E1 {
public:
	E1(T i) : ok(i) {

		cout << "ok: " << ok << endl ;
	}

	E1() : ok(T()) {

		cout << "ok: " << ok << endl ;
	}
private:	
		T ok;


};


int main() {
	E1<int> ee ;
	return 0 ;
}
