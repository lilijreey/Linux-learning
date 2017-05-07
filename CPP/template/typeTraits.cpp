/*
 *  Description: C++ 模板 traits 的实现技术，和相关主题
 *                        
 * set:vim foldmethod=marker
 */

#include <stdio.h>

//EE SFINAE(substitutin-failure-is-not-an-error) 
#if 0 // {{{
    //替换失败不是一个错误.
    //SFINAE就是在假如有一个物化可能产生编译错误的情况下，
    //如果还有其他选择，就忽略这个错误，进而选择其他的重载函数，而不会导致报错。
    //e.g

    void ss(int x) { printf ( "ss(int x) \n" ); }

    template <typename T>
    void ss(T x) {
	printf ( "template ss(T x)\n" );
    }

    //----------------------------------------
    void s1(int x) { printf ( "s1(int x) \n" ); }

    template <typename T, typename T::ok *T1=NULL>
    void s1(T x) {
	printf ( "template s1(T x)\n" );
    }
    //----------------------------------------
    void oo(int ) { //不需要使用的变量可以不指定行参
	printf ( "oo\n" );
    }

    int main() {
	double x=10.1;
	ss(x); //由于x是double根据匹配裁决不会调用int类型的ss
	s1(x); //这里就SFINAE 规则就会启用，和ss一样，对si的调用
	//会使用teample的s1但是，由于double类型没有ok成员，所以
	//编译器就会专用int的s1,如果没有SFINAE规则，就会报错。
	//模版变成中大两使用了SFINAE规则，的技术。
    }
#endif
// }}}

//////// Type Traits ///////////////////////////////////
//EE  检测是否是一个class类型
// {{{
#if 1
template  <typename T>
class IsClassT {
	typedef char One; //用于标记返回类型
	typedef struct {char a[2];} Two;

	//使用了SFINAE 机制，当一个非class类型调用test时，
	//返回的是One. 必须使用模版参数推到才能开启SFINAE机制
  //C++标准说只有当其它所有的重载版本都不能匹配时，具有任意参数列表(...)的重载版本才会被匹配。所以，如果T为类
	template<typename C> static One test(int C::*);
	template<typename C> static Two test(...);

	//如果T返回的是One，YES就是true
    public:
  //将0赋给任意类型指针都是合法的
	enum {YES = sizeof(IsClassT<T>::test<T>(0)) == 1};
	enum {NO = !YES};
};

    struct Ts {
    };

    int main() {
	if (IsClassT<Ts>::YES)
	    printf ( "TS is class\n" );
	else
	    printf ( "TS is non-class\n" );

	if (IsClassT<int>::YES)
	    printf ( "int is class\n" );
	else
	    printf ( "int is non-class\n" );
	
	    return 0 ;
    }
#endif
//}}}

/// bool 特化只能是 true、flase
// {{{
//template  <bool> struct is_conv; 
template  <int> struct is_conv ; 

//作用bool 和 int 一样作为非类型模板参数， 
//对于 下面的特化，不太模板参数就会导致不太的类型产生
//template<> struct is_conv<1> {};
//template<> struct is_conv<2> {}; // these two is_conv is not type

//由于没有对非类型参数的引用 如,所有上面的知道生成不同类型
template  <int x> struct is_conv; 

//??为啥 上面的 is_conv只是声明，没有定义
//因为这样做有个好处，只有对正确的类型特化后才可使用，没有特化的类型在使用时
//因为没有定义二会报错
//e.g. is_conv<4> x1; 
//
template<> struct is_conv<true> {};
//template<> struct is_conv<1> {};
//1 is conver to true
template<> struct is_conv<false> {};
//template<> struct is_conv<0> {};
//0 is conver to false

template< class Y, class T > struct sp_convertible
{
    typedef char (&yes) [1]; //声明一个 char[1] 类型的引用
    typedef char (&no)  [2]; //声明一个 char[2] 类型的引用
    static yes f( T* );  
    static no  f( ... );

	// (static_cast<Y*>(0)) 这个想必大家都能看懂，把0转成 Y*；
	// sizeof( (f)(Y*) ) 就是计算 f() 的返回值得大小，就是 yes 或no
	// 注意f( T* ) 是接受 T* 但是参数确实 static_cast<Y*>, 这里编译器就会
	// 进行隐士转换，如何Y* 和可以转换成 T* sizeof 就求的是 yes， 如果不能
	// 转换就是 no。下来容易了。 value 就算是 是否可以转换的结果。:)
    enum _vt { value = sizeof( (f)( static_cast<Y*>(0) ) ) == sizeof(yes) };
};
//}}}

// 静态  if
// {{{  有三个参数， 如果第一参数为true，则if:type 为第二个参数， 
// 否则if:type 为第三个参数
// 
// 实现原理： 偏特化一个个bool参数
template<
      bool C
    , typename T1
    , typename T2
    >
struct if_c
{
    typedef T1 type;
};

template<
      typename T1
    , typename T2
    >
struct if_c<false,T1,T2>
{
    typedef T2 type;
};

//使用； 从两个类型中达到sizeof大的
// typedef if_c<(sizeof(T1) > sizeof(T2)),
//      T1,
//      T2>::type MaxSizeType;
//      
// }}}

/// 把第一个参数可以转换成bool的类型 if
//  需要第一个类型必须有value成员
template<
      typename C
    , typename T1
    , typename T2
    >
struct if_
{
    typedef if_c<(bool)C::value, T1, T2> type;
};


//e.g. if_<true>::value
