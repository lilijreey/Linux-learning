/*
 * =====================================================================================
 *
 *       Filename:  i18n.cpp
 *
 *    Description:  This is about CPP internationnallizeon
 *
 *        Created:  11/05/2011 07:33:23 PM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */


#include	<iostream>
#include	<string>
#include	<locale>

using namespace std;

int main() {

	string str1("hello");

	char str2[10] = "world" ;
	str2[6] = '\0';
	str2[7] = '\0';
	str2[8] = '\0';
	str1.append(str2);

	cout << str1.length() << endl ;
}

