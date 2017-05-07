/*
 * =====================================================================================
 *
 *       Filename:  allocator.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/19/2011 09:57:37 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<iostream>
#include	<vector>
#include	<list>
#include	<exception>
#include	<limits> // for numerice_limits

#if 0
class 
enum
std::vector<int, MyAlloc<int> > s
std::map<int, 
	     string, 
		 less<int>, 
		 MyAlloc<int, string> > m
class EF{
public:
		int i 
			bool ;true false ;l
			std::using namespace ;
		value_type
			i = 4 + 4;
		new static_cast
			if 
			{"fesff\n"} ()esf				for
			else
new delete this friend using namespace
public protected private
bool wchar_t
inline virtual explicit export 
throw try catch
operator typeid
and bitor or xor compl bitand and_eq or_eq xor_eq not not_eq
 static_cast<fesf> mutable
syn keyword cppStructure	class typename template 
syn keyword cppBoolean		true false
}
std::basic_string<char, 
	              std::char_traits<char>, 
				  MyAlloc<char> > t
#endif
//
// create a alloctor
template <typename T>
class MyAlloc{	
public:
	typedef T  value_type;
	typedef T* pointer;
	typedef const T* const_pointer ;
	typedef T& reference ;
	typedef const T& const_reference ;
	typedef size_t size_type ;
	typedef ptrdiff_t difference_type;
	// rebind
	template <typename U>
	struct rebind{
		typedef MyAlloc<U> other;
	};

	MyAlloc() throw() : _buffer(0) { std::cout << "construct MyAlloc" << std::endl; }
	MyAlloc(const MyAlloc&) throw() { std::cout << "construct MyAlloc" << std::endl; }

	~MyAlloc() throw() ;

	size_type max_size() const { return std::numeric_limits<size_t>::max() / sizeof(T);}
	// the pHint is no used, this is deponed on impletement
	pointer allocate(size_type size, const void *pHint) ;

	void deallocator(pointer pos, size_type size); 

	// construct with T default constructor
//	void construct(pointer pos) ;
	// construct with val copy constructor
	void construct(pointer pos, const T& val) ;
//	void construct(pointer pos, T val, size_type size) ;

	//destory
	void destory(pointer pos) ;


	pointer address(reference val) { return &val; }
	const_pointer address(reference val) const { return &val; }

private:
	T* _buffer ;
};


template <typename T>
T*  MyAlloc<T>::allocate(size_type size, const void *pHint) {
	std::cout << "MyAlloc::allocator" << std::endl ;
	_buffer = static_cast<pointer>(::operator new(size * sizeof(T))) ;
	return _buffer ;
}

template <typename T>
void MyAlloc<T>::deallocator(pointer pos, size_type size)
{  
	std::cout << "MyAlloc::dealloctor" << std::endl ;
	::operator delete(pos) ;
}

template <typename T>	
void MyAlloc<T>::construct(pointer pos, const T& val) 
{
	std::cout << "MyAlloc::construct" << std::endl ;
	new((void*)pos) T(val) ;
}

template <typename T>	
void MyAlloc<T>::destory(pointer pos) 
{
	std::cout << "MyAlloc::destory" << std::endl ;
	pos->~T() ;
}


// compare two alloctor
template <typename T1, typename T2>
bool operator==(const MyAlloc<T1> &a1, const MyAlloc<T2> &a2)
{
	return true ;
}

template <typename T1, typename T2>
bool operator!=(const MyAlloc<T1> &a1, const MyAlloc<T2> &a2)
{
	return false ;
}
 
int main() {
	std::vector<int, MyAlloc<int> > v ;

}
