/**
 * @file     forward_list.cpp
 *           since C++11  使用单项列表实现
 *           和list相比他占用的空间更小，在不需要向后查询的操作时
 *           forward更好
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     05/29/2013 10:02:17 AM
 *
 */

///iterator is ForwardIterator 只提供++i, i++， 和==， * 操作

#include <forward_list>
#include <iterator>

//GCC4.6 实现
///Iter.
//_Fwd_list_node_base* _M_node; 指向node
// 
// 每个forward_list 都有一个头节点, 最后一个元素的next只为0
// 所有forward_list.end() 都相同，都是指向 0的Iter
// +------+ head  +----+
// | next | ... > | 0  |
// +------+       +----+
int main()
{
  ///construct
  std::forward_list<int> fl1= {1,2,3,4}; //supper initializer_list<T>
  std::forward_list<int> fl2(fl1.begin(), fl1.end());
  std::forward_list<int> fr3(4,1); //=={1,1,1,1}

  /// front
  //return 返回第一个元素的引用,如果forward_list为NULL，行为为定义
  printf("fornt:%d\n",fl1.front());

  ///begin() end(),所有forward的end值都相同（GCC的实现)
  if (fl1.begin() == fl2.begin())
    printf("fl1.begin == fl2.begin\n");

  if (fl1.end() == fl2.end())
    printf("fl2.end == fl2.end\n");

  ///before_begin() 
  //返回的是指向头地址的Iter，所以不过内容如何改变，before_begin 永远
  //不会改变
  /// emplace_after
  
  ///erase_after(iterator)
  ///erase_after(iteratorBegin, iteratorEnd)
  // 没有erase 返回
  
//  auto it = fl1.begin();
//  printf("begin next 2:%d\n", *std::next(it, 2));
//  std::advance(it, 2);
//  printf("begin advance 2:%d\n", *it);

  ///insert_after -> insert Iter
  //没有insert, 在指定的Iter后面插入
  printf("insert_after 11 in begin\n");
  fl1.insert_after(fl1.begin(), 11);
  for (auto i : fl1)
    printf("%d ", i);

  //想插入为第一个元素可以使用
  ///push_front.
  ///insert_after(fd1.before_begin(), 11)



  ///merge
  //merge 两个已经sort的forward_list
  
  ///unique
  ///sort
  //...

  

}
