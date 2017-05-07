/**
 * @file     algorithms.cpp
 *           
 *           c++11 算法
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     2015年05月31日 20时49分48秒
 *
 */

#include <algorithm>
#include <vector>
#include <list>
#include <cassert>

void test_all_of()
{
  //EE all_of 所有元素测试都返回true 才返回true, 其他false
  // std::all_of(inputItBegin, ItEnd, Predicate) //Predicate::fun(e) -> bool

  auto c1 = {1,2,3,4,5,6};
  std::vector<int> c2;

  std::all_of(c1.begin(), c1.end(), [](const int &e) { return e % 2 == 1;});

  //EE 如果是空 返回为true
  auto x = std::all_of(c2.begin(), c2.end(), [](const int &e) { return e % 2 == 1;});

  assert(x == true);
}

void test_any_of()
{
  //EE any_of 只要有一个元素返回true，就是true
  // std::any_of(inputItBegin, ItEnd, Predicate) //Predicate::fun(e) -> bool
   
  auto c1 = {1,2,3,4,5,6};
  std::vector<int> c2;

  std::any_of(c1.begin(), c1.end(), [](const int &e) { return e % 2 == 1;});

  //EE 如果是空 返回为false
  auto x = std::any_of(c2.begin(), c2.end(), [](const int &e) { return e % 2 == 1;});

  assert(x == false);
}

void test_none_of()
{
  ///EE  == not(all_of), 除了空
  //std::none_of(inputItBegin, ItEnd, Predicate) //Predicate::fun(e) -> bool
  //
  
  //EE 如果是空 返回为true
}


int
main(int argc, char *argv[])
{
  test_all_of();
  test_any_of();
  test_none_of();
  test_is_sorted();


  return 0;
}				/* ----------  end of function main  ---------- */

