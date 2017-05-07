/*
 *  Description: tuple 的使用
 */

//defined in "tuple" header file
#include <tuple>
#include <string>
#include <iostream>
using namespace std;

int main() {
	tuple<int, double, string> tt(10, 0.33, "hello");

	//non-member function
	//get<index>()
	std::cout << std::get<0>(tt) << " " ;
	std::cout << std::get<1>(tt) << "\n";

	return 0 ;
}
