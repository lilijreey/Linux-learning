/*
 * =====================================================================================
 *
 *       Filename:  memfunla.cpp
 *
 *    Description:  use function objects (functor)
 *
 *        Version:  1.0
 *        Created:  10/11/2011 09:21:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<string>
#include	<functional>
#include	<iostream>
#include	<vector>
#include	<algorithm>

class Person {
private:
	std::string name ;

public:
	 Person(std::string na="noname") : name(na) {}

	void print() const {
		std::cout << name << std::endl ;
	}

	void printWithPrefix(std::string prefix) const {
		std::cout << prefix << name << std::endl ;
	}
};

void foo(const std::vector<Person>& col1) {
	using std::for_each ;
	using std::bind2nd ;
	using std::mem_fun_ref ;

	for_each(col1.begin(), col1.end(), 
			 mem_fun_ref(&Person::print)) ;
// same to above
//	for (std::vector<Person>::const_iterator cit = col1.begin();
//			cit != col1.end(); ++cit) 
//		cit->print() ;
}

int main() {
	std::vector<Person> v ;
	v.push_back(Person("aa")) ;
	v.push_back(Person("bb")) ;
	v.push_back(Person("cc")) ;
	v.push_back(Person("dd")) ;
	v.push_back(Person("ee")) ;
	v.push_back(Person("ff")) ;
	foo(v) ;
}
