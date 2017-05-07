/*
 * =====================================================================================
 *
 *       Filename:  sort.cpp
 *
 *    Description:  
 *
 *        Created:  11/06/2011 08:51:48 AM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */

#include	<iostream>
#include	<vector>
#include	<string>
#include	<algorithm>
#include	<iterator> // for ostream_iterator

using namespace std ;

struct strtab_cmp
{
	typedef vector<char>::iterator strtab_vec_iterator;

	bool operator()(const pair<strtab_vec_iterator, strtab_vec_iterator> &x, 
					const pair<strtab_vec_iterator, strtab_vec_iterator> &y) const
	{
		return lexicographical_compare(x.first, x.second, y.first, y.second);
	}
};

struct strtab_print
{
	ostream &out;
	strtab_print(ostream &os) : out(os) {}

	typedef vector<char>::iterator strtab_vec_iterator ;
	void operator()(const pair<strtab_vec_iterator, strtab_vec_iterator> &s) const
	{
		copy(s.first, s.second, ostream_iterator<char>(out));
	}
};

int main() {
	vector<char> strtab ;
	char c;

	while (cin.get(c)) {
		strtab.push_back(c);
	}

	typedef vector<char>::iterator strtab_vec_iterator ;
	vector<pair<strtab_vec_iterator, strtab_vec_iterator> > lines ; // lines begin and end
	strtab_vec_iterator start = strtab.begin() ;
	
	while (start != strtab.end()) {
		strtab_vec_iterator next = find(start, strtab.end(), '\n');
		if (next != strtab.end())
			++next ;

		lines.push_back(make_pair(start, next)) ;
		start = next ;
	}

	// sort the lines
	sort(lines.begin(), lines.end(), strtab_cmp());

	ostream &ro = for_each(lines.begin(), lines.end(), strtab_print(cout)).out ;
}
