#ifndef _MY_UTILITY_H_
#define _MY_UTILITY_H_
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
#include  <stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<errno.h>

/* The marco name form E is error, V is has a variable.the means is use error test func
 * reutrn */

/* the mraco test a function if has error reture value, 
 * the error is error value,  rev is the function reture value, 
 * func is you called function, and arguments,  It must be
 *	func(arg, arg)
 * */
#if 0 //version 1.0
#define	EV_TEST(error, rev, func)   if((error) == ((rev) = (func))) {\
										printf("|%d|", __LINE__) ;\
										perror(#func) ;\
										exit(EXIT_SUCCESS) ;\
								}


#endif
// version 2.0

static inline void _print_line_(const char *file, unsigned int line) __attribute__((always_inline)) ;


/* @brief: base function */
static inline void _print_line_(const char *file, unsigned int line)
{
	(void)fprintf(stderr, "<%s:|%u|>\t", file, line) ;
}

#define EV_TEST(err, rev, func)       \
do{                                   \
 if ((err) == ((rev) = (func))) {     \
   _print_line_(__FILE__, __LINE__) ; \
   perror(#func) ;                    \
   exit(EXIT_FAILURE);                \
 }                                    \
} while (0) 

/*
 * the mraco test a function if error return value, 
 * but no rev 
 */
#define E_TEST(err, func)            \
do{                                  \
  if ((err) == (func)) {             \
  _print_line_(__FILE__, __LINE__) ; \
	perror(#func) ;                    \
	exit(EXIT_FAILURE);                \
  }                                  \
} while (0)

/*
 * the mrace test a function that it is only has seccuss retruen value.
 * T is success return value
 */
#define SV_TEST(success, rev, func)    \
do {                                   \
  if ((success) != ((rev) = (func))) { \
    _print_line_(__FILE__, __LINE__) ; \
    perror(#func) ;                    \
    exit(EXIT_FAILURE) ;               \
  }                                    \
} while(0)
											

/*
 * the mraco same as SV_TEST but not has rev
 */
#define S_TEST(success, func)          \
do {                                   \
  if((success) != (func)) {            \
    _print_line_(__FILE__, __LINE__) ; \
    perror(#func) ;                    \
    exit(EXIT_FAILURE) ;               \
  }                                    \
} while(0)

/* 
 * process pthread error function */
#define  PCHECK(func)                            \
do {                                             \
  int ret = (func);                              \
  if (ret != 0) {                                \
    _print_line_(__FILE__, __LINE__) ;           \
    fprintf(stderr, #func": %s", strerror(ret)); \
    exit(EXIT_FAILURE);                          \
  }                                              \
} while(0)

#define	stream stdout			/*  */
/* 
 * Trace message the macro can by print message to stderr
 * the macro is invirable and terminal that*/
#define trace_msg(stream, ...) _trace_msg(__FILE__, __LINE__, __func__, stream, __VA_ARGS__)

#define _trace_msg(file, line, func, stream, ...)                      \
do {                                                                   \
  (void)fputs("[TRACE Message]\ t", stream) ;                          \
	(void)fprintf(stream, "<%s:|%d| Function:%s>\ t", file, line, func); \
	(void)fprintf(stream, __VA_ARGS__); fputc('\ n', stream) ;           \
}while(0)

/*
 * Debug message
 */
#define debug_msg(stream, ...) _debug_msg(stream, __VA_ARGS__)

#define _debug_msg(stream, ...) \
do { \
  (void)fputs("[DEBUG Message]\t", stream) ;\
	(void)fprintf(stream, __VA_ARGS__); fputc('\n', stream) ; \
}while(0)

/* 
 * errno message the macro same as err_msg execpte print errno message to stderr */
#define errsys_exit(...) _errsys_exit(__FILE__, __LINE__, __func__,  __VA_ARGS__)

#define _errsys_exit(file, line, func, ...)   \
do { \
  (void)fputs("[ERROR Message]\t", stderr) ;\
	(void)fprintf(stderr, "<%s:|%d| Function:%s>\t", file, line, func) ;\
	(void)fprintf(stderr, __VA_ARGS__);\
	perror(0);\
	exit(EXIT_FAILURE) ; \
}while(0)


/* 
 * err_exit: print the  message and exit. the macro can by print debug message to stderr
 * the macro is invirable and terminal that*/
#define err_exit(...) _err_exit(__FILE__, __LINE__, __func__,  __VA_ARGS__)

#define _err_exit(file, line, func, ...) \
do { \
  (void)fputs("[ERROR Message]\t", stderr) ;\
	(void)fprintf(stderr, "<%s:|%d| Function:%s>\t", file, line, func) ;\
	(void)fprintf(stderr, __VA_ARGS__); exit(EXIT_FAILURE) ; \
}while(0)




#endif  /* tools.h */
