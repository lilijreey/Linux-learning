#ifndef _MY_UTILITY_H_
#define MY_UTILITY_H_
/*
 * =====================================================================================
 *
 *       Filename:  myUtility.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/20/2011 02:43:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

/* The marco name form E is error, V is has a variable.the means is use error test func
 * reutrn */

/* the mraco test a function if has error reture value, 
 * the error is error value,  rev is the function reture value, 
 * func is you called function, and arguments,  It must be
 *	func(arg, arg)
 * */
#define	EV_TEST(error, rev, func)   if((error) == ((rev) = (func))) {\
										printf("|%d|", __LINE__) ;\
										perror(#func) ;\
										exit(1) ;\
								}

/*
 * the mraco test a function if error return value, 
 * but no rev 
 */
#define E_TEST(error, func)		if((error) == (func)) {\
									printf("|%d|", __LINE__) ;\
									perror(#func) ;\
									exit(1); \
								}

/*
 * the mrace test a function that it is only has seccuss retruen value.
 * T is success return value
 */
#define SV_TEST(success, rev, func)		if((success) != ((rev) = (func))) {\
											printf("|%d|", __LINE__) ;\
											perror(#func) ;\
											exit(1) ;\
										}		

/*
 * the mraco same as SV_TEST but not has rev
 */
#define S_TEST(success, func)		if((success) != (func)) {\
											printf("|%d|", __LINE__) ;\
											perror(#func) ;\
											exit(1) ;\
										}
#endif 
