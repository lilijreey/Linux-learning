/*
 * =====================================================================================
 *
 *       Filename:  mapcmp.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/09/2011 08:51:45 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<iostream>
#include	<iomanip>
#include	<map>
#include	<string>
#include	<algorithm>

using namespace std ;

class Runt{
public:
	enum cmp_mode {normal, nocase} ;

private:
	const cmp_mode mode ;

	static bool nocase_compare(char c1, char c2) {
		return toupper(c1) < toupper(c2) ;
	}

public:
	Runt(cmp_mode m=normal) : mode(m) {}

	bool operator() (const string &s1, const string &s2) const {
		if (mode == normal) 
			return s1 < s2 ;
		else 
			return lexicographical_compare(s1.begin(), s1.end(), 
					s2.begin(), s2.end(), nocase_compare) ;
	}
	
};

typedef map<string, string, Runt> strmap ;
void fillAndPrint(strmap &coll) {
	coll["Deutschl"] = "Germany" ;
	coll["deutsch"] = "German" ;
	coll["Haken"] = "snag" ;
	coll["arbeiten"] = "work" ;
	coll["hund"] = "dog" ;
	coll["gehen"] = "go" ;
	coll["Unternehmen"] = "enter" ;
	coll["unternehmen"] = "under" ;
	coll["gehen"] = "walk" ;
	coll["estar"] = "undertaker" ;

	strmap::iterator pos ;
	for (pos = coll.begin();pos != coll.end(); ++pos) {
		cout << pos->first.c_str() << " " 
			<< pos->second << endl ;
	}
	cout << endl ;
}

int main() {
	strmap coll1 ;
	fillAndPrint(coll1) ;

	Runt ignorecase(Runt::nocase) ;
	strmap coll2(ignorecase) ;
	fillAndPrint(coll2) ;

}
