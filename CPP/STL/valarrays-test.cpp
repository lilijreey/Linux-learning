/*
 * =====================================================================================
 *
 *       Filename:  valarrays-test.cpp
 *
 *    Description:  about valarrays 
 *
 *        Version:  1.0
 *        Created:  10/01/2011 09:05:04 AM
 *       Revision:  noone
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<valarray>

#include	<iostream>
using std::endl ;
using std::cout ;

template <class T>
void print_valarray(const std::valarray<T> &va) {
	for (unsigned int i = 0; i < va.size(); ++i) {
		cout << "[" << i << "] = " << va[i] << ' ' ;
	}
	cout << endl ;
}

#if 0
/*
 * use member func: sum max min + = 
 */
int main() {
	std::valarray<int> va1(10) ;
	std::valarray<int> va2(2, 10) ; /* val, count */

	/* assign values 0 up to 9 to the first valarray */
	
	for ( unsigned int i = 0; i < va1.size(); i++ ) {
		va1[i] = i ;
	}

	/* assign -1 to all elements of the va2 */
	va2 = -1 ;

	cout << "va1" << endl ;
	print_valarray(va1) ;
	cout << "va2" << endl ;
	print_valarray(va2) ;

	/* print minimum, maximum, and sum of the first valarray */
	cout << "min(): " << va1.min() << endl ;
	cout << "max(): " << va1.max() << endl ;
	cout << "sum(): " << va1.sum() << endl ;

	/* assign values of the first to the sec. valarray */
	va2 = va1 ;

	/* remove all elements of the firest valarray */
	va1.resize(0) ;

	cout << "va1" << endl ;
	print_valarray(va1) ;
	cout << "va2" << endl ;
	print_valarray(va2) ;

	return 0 ;


}
#endif 



/*
 * use Treanscendental func (超越函数)
 * func: abs, exp, log, log10 sqrt, pow... 
 * It is in std scpeac
 */
int main() {
	std::valarray<double> va(9) ;
	
	/* assign values 0.0 up to 9.9 to the first valarray */
	for ( unsigned int i = 0; i < va.size(); i++ ) {
		va[i] = i * 1.1 ;
	}

	cout << "va" << endl ;
	print_valarray(va) ;

	std::valarray<double> vb(va + 10.0) ;
	std::valarray<double> vc(vb ) ;

	cout << "vb" << endl ;
	print_valarray(vb) ;

	vb = std::sqrt(vb) ;
	cout << "sqrt(vb)" << endl ;
	print_valarray(vb) ;


}
