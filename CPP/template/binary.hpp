#ifndef  BINARY_HPP_

#define  BINARY_HPP_
/*
 * =====================================================================================
 *
 *       Filename:  binary.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/26/2011 09:24:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */
//specilizic BinBit is only for 0 or 1
//为啥都是const呢，应为只有const常量可以在class内初始化
template <unsigned long b>
struct BinBit {
	//check it
//...
	static const unsigned bit = b % 10;
};



template <unsigned long N>
struct binary
{
	static unsigned const value = 
			binary<N/10>::value
			*2 + BinBit<N>::bit;
			//* 2 + N % 10 ;	// move the left bit to right
};

template<>
struct binary<0>
{
	static unsigned const value = 0 ;
};


#endif     /* ----- #ifndef BINARY_H_ ----- */
