/*
 *  Description: std::array
 *  defined in header<array>
 *  是对内置数组的STL容器风格的封装。
 *  vector有的member-func他都有。
 *  与vector不同1.array支持0个元素的变量
 *              2.array多数的操作都是编译期执行的
 */

#include <array>
#include <stdio.h>

using namespace std;

//class 中使用
struct A
{
  //??? 如何初始化 数组
//  A(initializer_list<int> b)
//      :a(b)
//  {}
//
//  int a[4];
};

#if 0
int main() {
  std::array<int, 3> a{ {1,2,3} };
  printf("%d\n", a[1]);
  std::swap(a[0], a[2]);
  printf("%d\n", a[2]);
    //vector like op 
//    std::array<int, 5> ary;  //create a 5 element int 
    //printf ( "%d",ary._M_instance[1]);

    //int cary[] = { 1,3,4};
    //4.6 not support not number spicfiy e.g array<T> 

    //empty, size, max_size:标准 上说返回可以容纳的最大元素数，
    //gcc4.6 的实现是返回 ary声明时的（5）但是返回
//    if (ary.empty()) printf ( "ary is empty\n" );
//    printf ( "ary size is %d max_size:%d\n", ary.size(),ary.max_size());
//    int v=0;
//    //begin, end
//    for (auto i=ary.begin(); i!= ary.end(); ++i, ++v) {
//	*i=v;
//	printf ( "%d ", *i );
//    }
//
//    //member func: front back
//    printf("front:%d, back:%d\n", ary.front(), ary.back());
//    //at, []
//    printf ( "ary[2]:%d, ary.at():%d\n",ary[2], ary.at(3));
//    //data //fill 把array全部元素设为一个值
//    //
//
//
//    //non-member functions
//    //== != < > <= >=
//    //std::get
//    printf ( "ary[1]=%d\n",get<1>(ary) );

    return 0 ;

    //tuple_size get array size
}
#endif


#if 0
  /** GCC 4.7 impletate
   *  @brief A standard container for storing a fixed size sequence of elements.
   *
   *  @ingroup sequences
   *
   *  Meets the requirements of a <a href="tables.html#65">container</a>, a
   *  <a href="tables.html#66">reversible container</a>, and a
   *  <a href="tables.html#67">sequence</a>.
   *
   *  Sets support random access iterators.
   *zR
   *  @param  Tp  Type of element. Required to be a complete type.
   *  @param  N  Number of elements.
  */
  template<typename _Tp, std::size_t _Nm>
    struct array
    {
      typedef _Tp 	    			      value_type;
      typedef _Tp*                                    pointer;
      typedef const _Tp*                              const_pointer;
      typedef value_type&                   	      reference;
      typedef const value_type&             	      const_reference;
      typedef value_type*          		      iterator;
      typedef const value_type*			      const_iterator;
      typedef std::size_t                    	      size_type;
      typedef std::ptrdiff_t                   	      difference_type;
      typedef std::reverse_iterator<iterator>	      reverse_iterator;
      typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;

      // Support for zero-sized arrays mandatory.
      value_type _M_instance[_Nm ? _Nm : 1];

      // No explicit construct/copy/destroy for aggregate type.

      // DR 776.
      void
      fill(const value_type& __u)
      { std::fill_n(begin(), size(), __u); }

      void
      swap(array& __other)
      { std::swap_ranges(begin(), end(), __other.begin()); }

      // Iterators.
      iterator
      begin()
      { return iterator(std::__addressof(_M_instance[0])); }

      const_iterator
      begin() const 
      { return const_iterator(std::__addressof(_M_instance[0])); }

      iterator
      end()
      { return iterator(std::__addressof(_M_instance[_Nm])); }

      const_iterator
      end() const
      { return const_iterator(std::__addressof(_M_instance[_Nm])); }

      reverse_iterator 
      rbegin()
      { return reverse_iterator(end()); }

      const_reverse_iterator 
      rbegin() const
      { return const_reverse_iterator(end()); }

      reverse_iterator 
      rend()
      { return reverse_iterator(begin()); }

      const_reverse_iterator 
      rend() const
      { return const_reverse_iterator(begin()); }

      const_iterator
      cbegin() const 
      { return const_iterator(std::__addressof(_M_instance[0])); }

      const_iterator
      cend() const
      { return const_iterator(std::__addressof(_M_instance[_Nm])); }

      const_reverse_iterator 
      crbegin() const
      { return const_reverse_iterator(end()); }

      const_reverse_iterator 
      crend() const
      { return const_reverse_iterator(begin()); }

      // Capacity.
      constexpr size_type 
      size() const { return _Nm; }

      constexpr size_type 
      max_size() const { return _Nm; }

      constexpr bool 
      empty() const { return size() == 0; }

      // Element access.
      reference
      operator[](size_type __n)
      { return _M_instance[__n]; }

      const_reference
      operator[](size_type __n) const
      { return _M_instance[__n]; }

      reference
      at(size_type __n)
      {
	if (__n >= _Nm)
	  std::__throw_out_of_range(__N("array::at"));
	return _M_instance[__n];
      }

      const_reference
      at(size_type __n) const
      {
	if (__n >= _Nm)
	  std::__throw_out_of_range(__N("array::at"));
	return _M_instance[__n];
      }

      reference 
      front()
      { return *begin(); }

      const_reference 
      front() const
      { return *begin(); }

      reference 
      back()
      { return _Nm ? *(end() - 1) : *end(); }

      const_reference 
      back() const
      { return _Nm ? *(end() - 1) : *end(); }

      _Tp*
      data()
      { return std::__addressof(_M_instance[0]); }

      const _Tp*
      data() const
      { return std::__addressof(_M_instance[0]); }
    };

  // Array comparisons.
  template<typename _Tp, std::size_t _Nm>
    inline bool 
    operator==(const array<_Tp, _Nm>& __one, const array<_Tp, _Nm>& __two)
    { return std::equal(__one.begin(), __one.end(), __two.begin()); }

  template<typename _Tp, std::size_t _Nm>
    inline bool
    operator!=(const array<_Tp, _Nm>& __one, const array<_Tp, _Nm>& __two)
    { return !(__one == __two); }

  template<typename _Tp, std::size_t _Nm>
    inline bool
    operator<(const array<_Tp, _Nm>& __a, const array<_Tp, _Nm>& __b)
    { 
      return std::lexicographical_compare(__a.begin(), __a.end(),
					  __b.begin(), __b.end()); 
    }

  template<typename _Tp, std::size_t _Nm>
    inline bool
    operator>(const array<_Tp, _Nm>& __one, const array<_Tp, _Nm>& __two)
    { return __two < __one; }

  template<typename _Tp, std::size_t _Nm>
    inline bool
    operator<=(const array<_Tp, _Nm>& __one, const array<_Tp, _Nm>& __two)
    { return !(__one > __two); }

  template<typename _Tp, std::size_t _Nm>
    inline bool
    operator>=(const array<_Tp, _Nm>& __one, const array<_Tp, _Nm>& __two)
    { return !(__one < __two); }

  // Specialized algorithms [6.2.2.2].
  template<typename _Tp, std::size_t _Nm>
    inline void
    swap(array<_Tp, _Nm>& __one, array<_Tp, _Nm>& __two)
    { __one.swap(__two); }

  // Tuple interface to class template array [6.2.2.5].

  /// tuple_size
  template<typename _Tp> 
    class tuple_size;

  /// tuple_element
  template<std::size_t _Int, typename _Tp>
    class tuple_element;

  template<typename _Tp, std::size_t _Nm>
    struct tuple_size<array<_Tp, _Nm> >
    { static const std::size_t value = _Nm; };

  template<typename _Tp, std::size_t _Nm>
    const std::size_t
    tuple_size<array<_Tp, _Nm> >::value;  

  template<std::size_t _Int, typename _Tp, std::size_t _Nm>
    struct tuple_element<_Int, array<_Tp, _Nm> >
    { typedef _Tp type; };

  template<std::size_t _Int, typename _Tp, std::size_t _Nm>
    inline _Tp&
    get(array<_Tp, _Nm>& __arr)
    { return __arr[_Int]; }

  template<std::size_t _Int, typename _Tp, std::size_t _Nm>
    inline const _Tp&
    get(const array<_Tp, _Nm>& __arr)
    { return __arr[_Int]; }

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace
#endif


