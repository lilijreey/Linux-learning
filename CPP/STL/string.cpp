/*
 * =====================================================================================
 *
 *       Filename:  string.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/17/2011 09:41:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<string>

#include	<iostream>

int main() {
	// the string wstring is the basic_string specialization
	std::basic_string<int> aa(34, 2) ;
	std::cout << aa[13] << std::endl ;
}

