/*
 * =====================================================================================
 *
 *       Filename:  map.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/09/2011 06:57:46 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#include <cstdio>
#include	<map>
#include	<utility>
#include	<string>

//EE lower_bound
//upper_bound

int main()
{
  std::multimap<int,int> m = {{1,2},{3,4},{4,5},{6,7},{4,6}};

  //lower_bound N 返回map中第一个>= N 的it
  auto lowerIt = m.lower_bound(3); // 3 ....

  while (lowerIt != m.end()) {
    printf("k:%d v:%d\n", lowerIt->first, lowerIt->second);
    ++lowerIt;
  }

  

}

