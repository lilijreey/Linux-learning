/*
 * =====================================================================================
 *
 *       Filename:  xx.cpp
 *
 *    Description:  
 *
 *        Created:  01/06/2012 01:41:51 AM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */

#include	<iostream>
#include	<set>
#include	<list>
#include	<vector>
#include	<algorithm>
#include    <boost/unordered/unordered_set.hpp>
extern "C"
{
#include	<unistd.h>
#include	<sys/time.h>
}

//#include	<boost/unordered.hpp>

void ca(int i) {++i;}

#define conut 1000000
int main() {
	std::vector<int> aa ;
	aa.reserve(conut) ;

	for (int i=0; i < conut; ++i) {
		aa.push_back(i) ;
	}

	std::cout << "________VECTOR_________________________________________________" <<std::endl ; 
	struct timeval tv ;
	struct timeval tv1;
	(void)gettimeofday(&tv, NULL) ;
	for(std::vector<int>::iterator it = aa.begin();
			it != aa.end(); ++it) ca(*it) ; 

	(void)gettimeofday(&tv1, NULL) ;
	std::cout << "tv_sec:" << tv1.tv_sec - tv.tv_sec << " usec:" << tv1.tv_usec - tv.tv_usec << std::endl ; 
	//for_each
	std::cout << "________VECTOR___FOR_EACH______________________________________________" <<std::endl ; 
	(void)gettimeofday(&tv, NULL) ;
	for_each(aa.begin(), aa.end(), ca) ; 
	(void)gettimeofday(&tv1, NULL) ;
	std::cout << "tv_sec:" << tv1.tv_sec - tv.tv_sec << " usec:" << tv1.tv_usec - tv.tv_usec << std::endl ; 

	std::cout << "________LIST_________________________________________________" <<std::endl ; 

	std::list<int> bb ;
	for (int i=0; i < conut; ++i) {
		bb.push_back(i) ;
	}
	(void)gettimeofday(&tv, NULL) ;
	for(std::list<int>::iterator it = bb.begin();
			it != bb.end(); ++it)  ca(*it) ; 
	(void)gettimeofday(&tv1, NULL) ;
	std::cout << "tv_sec:" << tv1.tv_sec - tv.tv_sec << " usec:" << tv1.tv_usec - tv.tv_usec << std::endl ; 

	std::cout << "________LIST___FOR_EACH______________________________________________" <<std::endl ; 
	(void)gettimeofday(&tv, NULL) ;
	for_each(bb.begin(), bb.end(), ca) ; 
	(void)gettimeofday(&tv1, NULL) ;
	std::cout << "tv_sec:" << tv1.tv_sec - tv.tv_sec << " usec:" << tv1.tv_usec - tv.tv_usec << std::endl ; 

	std::cout << "______MAP___________________________________________________" <<std::endl ; 
	std::set<int> cc ;
	for (int i=0; i < conut; ++i) {
		cc.insert(i) ;
	}
	(void)gettimeofday(&tv, NULL) ;
	for(std::set<int>::iterator it = cc.begin();
			it != cc.end(); ++it)  ca(*it);
	(void)gettimeofday(&tv1, NULL) ;

	std::cout << "tv_sec:" << tv1.tv_sec - tv.tv_sec << " usec:" << tv1.tv_usec - tv.tv_usec << std::endl ; 

	std::cout << "________MPA___FOR_EACH______________________________________________" <<std::endl ; 
	(void)gettimeofday(&tv, NULL) ;
	for_each(cc.begin(), cc.end(), ca) ; 
	(void)gettimeofday(&tv1, NULL) ;
	std::cout << "tv_sec:" << tv1.tv_sec - tv.tv_sec << " usec:" << tv1.tv_usec - tv.tv_usec << std::endl ; 

	std::cout << "______HASH set___________________________________________________" <<std::endl ; 
	boost::unordered_set<int> dd ;
	for (int i=0; i < conut; ++i) {
		dd.insert(i) ;
	}

	(void)gettimeofday(&tv, NULL) ;
	for(boost::unordered_set<int>::iterator it = dd.begin();
			it != dd.end(); ++it)  ca(*it);
	(void)gettimeofday(&tv1, NULL) ;
	std::cout << "tv_sec:" << tv1.tv_sec - tv.tv_sec << " usec:" << tv1.tv_usec - tv.tv_usec << std::endl ; 

	std::cout << "______HASH_____FOR_EACH______________________________________________" <<std::endl ; 

	(void)gettimeofday(&tv, NULL) ;
	for_each(dd.begin(), dd.end(), ca) ; 
	(void)gettimeofday(&tv1, NULL) ;
	std::cout << "tv_sec:" << tv1.tv_sec - tv.tv_sec << " usec:" << tv1.tv_usec - tv.tv_usec << std::endl ; 
	return 0 ;
}

