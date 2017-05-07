//gcc 4.6 implement

// what id begin() ?
// what id end() ?
// Is end() constant?

///vector 存数据的空间是malloc分配的
//vector使用3个指针管理， 
//  _M_start = begin()
//  _M_finish = end()
//  _M_end_of_storage = _M_start + size

/// vector 的move Construct
//    _Vector_base(_Vector_base&& __x)
//        : _M_impl(__x._M_get_Tp_allocator())
//    {
//        this->_M_impl._M_start = __x._M_impl._M_start;
//        this->_M_impl._M_finish = __x._M_impl._M_finish;
//        this->_M_impl._M_end_of_storage = __x._M_impl._M_end_of_storage;
//        //把old设置为0
//        __x._M_impl._M_start = 0;
//        __x._M_impl._M_finish = 0;
//        __x._M_impl._M_end_of_storage = 0;
//    }
//    
#include	<vector>
#include	<deque>
#include	<list>
#include	<iostream>
using namespace std ;

int main() {
	vector<int> v ;
	deque<int> d ;
	list<int> l ;
	cout << "v.maxsize(): " << v.max_size() << endl ;
	cout << "v.capacity(): " << v.capacity() << endl ;
	cout << "d.max_size(): " << d.max_size() << endl ;
	cout << "l.max_size(): " << l.max_size() << endl ;

	v.reserve(100) ;
	cout << "v.maxsize(): " << v.max_size() << endl ;
	cout << "v.capacity(): " << v.capacity() << endl ;

	/* reserve less then it own, the reserve not effect */
  //	v.reserve(10) 
  //	cout << "v.maxsize(): " << v.max_size() << endl ;
  //	cout << "v.capacity(): " << v.capacity() << endl ;

	v.assign(10, 4) ;
	cout << "v.maxsize(): " << v.max_size() << endl ;
	cout << "v.capacity(): " << v.capacity() << endl ;

  ///构造两个空vector比较他们的begin(), end() 
  vector<int> e1, e2;
  if (e1.begin() == e2.begin()) //same 
    printf("空的vector，begin（） 相同\n");
  else
    printf("空的vector，begin（） NO相同\n");

  //只要一分配空间begin() 赋值后就不同了
  e1.reserve(10);
  e2.reserve(10);
  printf("e1.size()%ld \n", e1.size());
  if (e1.begin() == e2.begin()) //same 
    printf("空的vector，begin（） 相同\n");
  else
    printf("空的vector，begin（） NO相同\n");

}
