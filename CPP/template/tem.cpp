
#include <iostream>
#include <deque>
#include <list>
using namespace std;
// 强@@@，
// 第二个参数就是你想要的模板的原型，注：template，class，因为第二
// 个参数是模板，所有用template，CO 是一个类而不是类型
template <typename T, typename CONE = std::deque<T> >
class Cc{
    public:
        CONE a;
    void show() {std::cout << "cc" << std::endl;}
};

template <typename T, 
template <typename X, typename Alloc> class CO  >
class C1{
    public:
        CO<T, allocator<T> > a ;
    void show() {std::cout << "C1" << std::endl;}
};

template <typename T, template <typename X, typename Alloc> class CO = std::list  >
class C2 {
public:
    CO<T, allocator<T> > a ;
    void show() {std::cout << "C2" << std::endl;}

};

template <typename T, template <typename X, typename Alloc = std::allocator<X> > class CO = std::list  >
class C3 {
    public:
    CO<T> a ; // ^_^ 
    void show() {std::cout << "C3" << std::endl;}
};


enum  TT{A,B,C} ;
template <TT T>
class Ttal{
	public: 
		virtual void xu()=0 ;
		Ttal(TT i) : x(i) {std::cout << "c Taal x:" << x <<std::endl ; };
		TT x ;
};

//class it 
//: public  Ttal<44> {
//	public:
//	it()  : Ttal<>(A) {std::cout << "c it x:" << x <<std::endl ;}
//	void xu() {} ;
//};
//
class te {
	public:	

	te() { std::cout <<	"te construction" << std::endl ; }
	~te() { std::cout <<	"te disstruction" << std::endl ; }
	void sh() const {
		std::cout <<	"shw" << std::endl ; 
	}
};

void xt(te xx) {
	xx.sh();
}


//特化，和虚继承
//如果我有一个抽象化的接口需要根据不同的实体，体现不同的实现
//我可以用虚函数，或者模板特化

//种族，更加不同种族show有不同的方法
enum RACE {
	MONSTER,
	HUMAN,
};

template <RACE> 
struct Role;
//{ 公共接口，所有特化类必须实现以下接口
// void show() ;
//};

template<>
struct Role<MONSTER> {
	void show() { std::cout <<	"Role is monster" << std::endl ; }
};

template<>
struct Role<HUMAN> {
	void show() { std::cout <<	"Role is human" << std::endl ; }
};


enum TYPE {
	AT,
	BT,
};

template <TYPE>
struct typePolicy ;

template <> struct typePolicy<AT> 
{static const int aa=1;};

template <> struct typePolicy<BT> {
	static const int aa=2;       };

/*  
不能直接使用 
template <TYPE>
因为，这样的话是数值模板，不是类型模板无法通过编译
*/

//下面的都可以达到根据不同类型初始化不同值
//double template
//template <typename tt, tt t1>
//template <TYPE t, template <TYPE> class CO >
template <TYPE t>
struct RoInf{
	//模板函数如何写???????
//template <typename tt, tt t1>
//template <TYPE t, template <TYPE> class CO >
	RoInf() : aa(typePolicy<t>::aa){ 
	}
	int aa;

	void show() {std::cout << aa << ":" << std::endl ; }
};

//构造函数为模板函数 ?????,非类型和类型模板参数对于
//成员模板的影响????
struct TempConstrctor {
	template<typename T >
		//如果有一个模板参赛但是我不用会怎样??
		explicit TempConstrctor() :a(){} ;
	void show() {std::cout <<	a << std::endl ; }

	int a;
};

//继承模板类
template <typename  T, T vv>
struct InBase ;

//template <>
//struct InBase<int 3> {
//	InBase() :val(vv) {}
//	int val;
//};

//template <typename  int, int 4>
//struct InBase {
//	InBase() :val(vv) {}
//	T val;
//};
//
////继承 InBase
//struct Inh : private InBase<int,4>
//{
//	Inh():InBase<int,4>() {}
//};

template <typename T>
void ss() {
	std::cout << "SS" <<std::endl ; 
}
//当一个模板参赛不用时，编译器无法推到除来，只能指定
template <int i >
//和 template<int> 的区别？？？？？？
void sx() {
	std::cout << "SX" <<std::endl ; 
}
//sx<3>();
//ss<int>();


int main() {
	//这里不能用 xx() 这样会导致编译器认为这是一个函数声明
	RoInf<AT> ok;
	ok.show();

}
