/*
 * =====================================================================================
 *
 *       Filename:  accum6.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/05/2011 12:08:01 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#ifndef  SUMPOLICY1_HPP_
#define  SUMPOLICY1_HPP_

class Sum_policy {
public:
	template <typename T1, typename T2>
	static void accumulate(T1 &total, T2 const &val) {
		total += val ;
	}
};

#endif     /* ----- #ifndef SUMPOLICY1_H_ ----- */
