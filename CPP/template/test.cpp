/*
 * =====================================================================================
 *
 *       Filename:  test.cpp
 *
 *    Description:  about chapter1 
 *
 *        Version:  1.0
 *        Created:  10/02/2011 09:13:09 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#include	<iostream>
#include	<string>
#include	<vector>
#include	<list>
#include	<functional>
#include	<algorithm>

using namespace  std;
#if 0
/* func template 
 * @suppter: '<' operator*/

template <typename T>
inline const T& max(T const &a, T const &b) ;

template <typename T1, typename T2>
inline const T1 max1(T1 const &a, T2 const &b) ;

template <typename T>
const T& max(T const &a, T const &b) {
	return a < b ? a : b ;
}

template <typename T1, typename T2>
const T1 max1(T1 const &a, T2 const &b) {
	std::cout << "T1:T2 " << a << ":" << b << std::endl ;
	return a < b ? a : b ;
}

class Ex {
	friend std::ostream& operator<<(std::ostream &os, const Ex &e) ;
	private:
		int a ;
	public:
		bool operator<(const Ex &e) const { return true ; }

};

std::ostream& operator<<(std::ostream &os, const Ex &e) {
	os << e.a ;
	return os ;
}
int main() {
	int i = 32 ;
	/* 对于max(int, double)不会完全匹配有三种方法来解决：
	 * 1.需要静态转换 */
	std::cout << "max(7, 1): " << ::max(static_cast<int>(3.3), i) << std::endl ;
	/* 2.specified int type 
	 *   ::max<int>(4.2, 2)
	 * 3. max(T1, T2) 分别定义不同的参数类型 max1 */

	std::cout << "max(7, 1): " << ::max1(i, 3.3) << std::endl ;

	/*
	 * 4.对于返回只，可以再定义一个类型，调用函数是指定返回值
	 * e.g template <typename RT, typename T1,  typename T2>
	 *			RT max(const T1 &a, const T2 &b) {...}
	 *	called:  ::max<double>(34.3, 233) ;
	 */
	

	Ex e1, e2 ;
	std::cout << "max(e1, e2): "<< ::max(e1, e2) << std::endl ;
	
	std::string str1("233"), str2("hellO") ;
	std::cout << "max(str1, str2): " << ::max(str1, str2) << std::endl ;



	return 0 ;
}
#endif

/* overload 优先non-tempaltefunc */
/* 自动类型转换只在non-template中，template中没有 */
#if 0
template <typename T> int f(T) 
{std::cout << "template" << std::endl; return 0 ; }
void f(int) {std::cout << "int" << std::endl; }
int main() {
	int x = 3 ;
	return f(x) ;
}
#endif

/* Class Template */

#include	<vector>
template <typename T>
class Stack{
	private:
	std::vector<T> elems ;

	public:
	Stack() ;

	template <typename T2>
	Stack<T>& operator=(Stack<T2> const &t2) ;
	
	void push(T const &) ;
	void pop() ;
	T top() const ;
	bool empty() { return elems.empty(); }
} ;


template <typename T>
void
Stack<T>::push(T const &elem) {
	elems.push_back(elem) ;
}

template <typename T>
void
Stack<T>::pop() {
	if (! elems.empty()) {
		elems.pop_back() ;
	}
}

 
template <typename T>
T
Stack<T>::top() const {
	if (elems.empty())
		return 0 ;
	return elems.back() ;
}
/* Specificizsion */
#if 0
template<> /* specificizistion */
class Stack<int> {
private:
	std::vector<int> elems ;
public:
	void push(int ) ;
	void pop() ;
	int top() const ;
	bool isempty() const {
		return elems.empty() ;
	}
} ;

void
Stack<int>::push(int elem) {
	elems.push_back(elem) ;
}

void
Stack<int>::pop() {
	if (! elems.empty()) {
		elems.pop_back() ;
	}
}

int 
Stack<int>::top() const {
	if (elems.empty())
		return 0 ;
	return elems.back() ;
}
#endif


/* 在成员函数中使用this->x调用基类中的方法，而不是x，  */
#if 0

template <typename T>
class Ba {
	public:
		void exit(){ std::cout << "Ba::exit()" << std::endl ;}
} ;

template <typename T>
class Ex : public Ba<T> {
	public:
		void foo() {
			std::cout << "Ex::foo()" << std::endl ;
			this->exit() ;
		}


};

int main() {
	Ex<int> e ;
	e.foo() ;
	return 0 ;
}
#endif

/* assignment temple 允许不同类型的templateClass可以复制 */
/* add a template assignment operator */
/* the template assignmnet 不会取代默认的operator= */
/*
 * class Stack {
 *		....
 *		template <typename T2>
 *		Stack<T>& operator=(Stack<T2> const &t2) ;
 * }
 */

#if 0
template <typename T>
 template <typename T2>
Stack<T>& 
Stack<T>::operator=(Stack<T2> const &t2) {
	//	无用因为不会在两个相同类型之间调用，更别说是一样的对象了
//	if ((void *)this == (void *)&t2)
//		return *this ; 

	Stack<T2> tmp(t2) ;
	elems.clear() ;
	elems.empty() ;
	while (!tmp.empty()) {
		elems.push_back(tmp.top()) ;
		tmp.pop() ;
	}
	return *this ;
}

int main() {
	Stack<int> st1 ;
	Stack<int> st2 ;
	Stack<double> st3 ;

	st1 = st2 ;
//	st2 = st3 ; // no match
	return 0 ;
}
#endif

// template function dufalut parametes
#if 0
template <class T>
inline void ok(T x=1) { 
	std::cout << x << std::endl ;
}

template <class T1, class T2>
inline void kk(T1 x, T2 y) { 
	std::cout << x << std::endl ;
}
template<>
//void ok(int x=4) { // error :default argument specified in exlicit specialization
void ok(int x) {
	std::cout << "int:" << x << std::endl ;
}
int main() {
	ok<int>() ; // this is 1
	ok<>(4); // use template
	kk(1 , 2);
}
#endif

template <typename T>
void init() {
	T x = T() ; // int is 0
	std::cout << x << std::endl ;
}
//

// shallow intantiation
#if 0
template <typename T>
void clear(T const &p) {
	*p = 0;
}

template <typename T>
void core(T const &p) {
	clear(p) ;
}

template <typename T>
void middle(typename T::Index p) {
	core(p) ;
}

template <typename T>
void shell(T const &env) {
	typename T::Index i ;
	middle<T>(i) ;
}

class Cl {
public:
	typedef int Index ;
};

Cl main_cl ;

int main() {
	shell(main_cl) ;
}

#endif 

// traits
#if 0
template <typename T>
void show(T x) {
	std::cout << "maunal" <<x << std::endl ; 
}

// to point the show T* is better  show T
template <typename T>
void show(T *x) {
	std::cout << "proint" << x << std::endl ; 
}
template <typename T>
void show(const T *x) {
	std::cout << "const proint" << x << std::endl ; 
}

template <typename T>
void show(T **x) {
	std::cout << "p proint" << x << std::endl ; 
}

// inhirtase std::iterator
template <typename T>
class myIter : 
	public std::iterator<std::forward_iterator_tag, T> {
	

	};
int main() {
	int i = 4 ;
	int *p = &i ;
	int const *cp = &i ;
	int **pp = &p ;
	show(i) ;
	show(p) ;
	show(cp) ;
	show(pp) ;
}
#endif

// functional mem_fun use to foreach

#if 0
struct X  {
	X() : i(10) {}
	int i ;
	void show() {std::cout << i << "\n" ;}
	void set(int m) {  i =m; }
};

void show(const X &r) {
	std::cout << r.i << "\n";
}

struct funal {
	void operator()(const X &r) {
		std::cout << r.i << "\n";
	}
};
// 对于for_each 传入的第三的参数不能是成员函数
// 但是可以用fun_mem 包装此成员函数，使得可以传入
int main() {
	std::vector<X> vc ;
	vc.push_back(X());
	vc.push_back(X());
	vc.push_back(X());
	vc.push_back(X());
	vc.push_back(X());

	std::cout << " use nonmember-function\n" ;
	for_each(vc.begin(), vc.end(), show);

	std::cout << "use function objction\n" ;
	funal f;
	for_each(vc.begin(), vc.end(), f);

	std::cout << "use member-function\n";
	for_each(vc.begin(), vc.end(), std::mem_fun_ref(&X::show));
	// conenton is fill obj use ref ,  fill pointer use mem_fun
	// mem_fun... 中的函数只能用于无参函数
	//for_each(vc.begin(), vc.end(), std::mem_fun_ref(&X::set(3)));
	return 0;
}
#endif


//多继承，基类间调用
//不能相互调用，英文没有对象，所以说，基类
//只能提供正交接口，子类来调用

#if 0
struct SO {
	SO():i(0){}
	void addi(int v) { i+= v;}
	int geti() const {return i;}
	int i;
};

struct Str {
	void att() { addi(20);}
	void show() {showb(), std::cout <<	geti() <<std::endl ; }
};

struct BU {
	void showb() {std::cout << "buf" <<std::endl ; }
};
#endif

//根据 id特话模板函数
//typedef 模板


template <int opId>
void handle() {
	std::cout << "no speci:" <<	opId <<std::endl ; 
	handle<opId-1>();
}

template <> 
void handle<1>() {
	std::cout <<"h 1" <<std::endl ; 
}

//只用是非类型模板汉斯，不用模板typede也可以
typedef void(*hh)();

//把handl安装到handle表中

vector<hh> htable;

//最好自动生成
void install() {
	htable.push_back(handle<1>);
	htable.push_back(handle<2>);
	htable.push_back(handle<3>);
};

void nu() {std::cout <<	"un" <<std::endl ; }

void action(size_t i) {

	if (i > htable.size()) {
		std::cout <<"i is bad id" <<std::endl ; 
		return ;
	}

	htable[i-1]();
}

typedef list<int> linex;
int main() {
	install();
	std::cout <<	"install finished" <<std::endl ; 

	action(3);
}
