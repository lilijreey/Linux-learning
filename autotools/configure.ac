dnl # The simplest possible configure.ac has just two line.
dnl # 这些宏都定义在autoconf/*.m4 文件中

dnl # 需要的autoconf 最低版本
AC_PREREQ([2.61])

# 初始化 Autoconf
AC_INIT([Jupiter],[v0.1], [lil@gmail.com])

## 可选 定义辅助的工具脚本输出目录 e.g. install-sh ..
## 默认是当前目录
AC_CONFIG_AUX_DIR([build-aux])

## 初始化automake
##   打开所有的警告, 
##    foreign 会忽略一些GNU 标准文件 e.g. INSTALL NEWS 
##   -Wall 打开所有的警告
##   -Werror 把警告当作错误
AM_INIT_AUTOMAKE([foreign 
                  -Wall -Werror])

## 参数是一个src file，to make sure the the configure 
## is not run from other space
AC_CONFIG_SRCDIR([src/main.c])

## 检查 是否有一个C 编译器
AC_PROG_CC
# AC_PROG_CC 接受一个可选的c编译器的命令列表
# AC_PROG_CC([cc cl gcc])
# AC_PROG_CC 会定义几个宏

#@CC@ (full path of compiler)
#@CFLAGS (eg., -g -O2 for gcc)
#@CPPFLAGS@ (empty by default)
#@EXEEXT@ (eg., .exe)
#@OBJEXT@ (eg., .o)

# 在生成Makefile.in 的时候
# Makefile 的CFLAGS 会用 @CFLAGS@ 赋值
# 所有如果要改变默认的 CFLAGS 可以赋值

## AC_PROG_CC_C_O automake 需要使用

## check c++ 
## AC_PROG_CXX

## declare 申明 config.h 是一个 输出头文件
AC_CONFIG_HEADERS([config.h])

## declare Makefile 和 src/Makefile 作为输出文件
## Create file.in for each file.am
AC_CONFIG_FILES([Makefile src/Makefile])

## Checks for header files.
## and define HAVE_HEADER_H for each header found
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

## Checks exists lib and contains func 
## and execute found if it does, not-found otherwise
# AC_CHECK_LIB(lib, funct, [act-if-found], [act-if-not])


dnl Checks for basic compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_C_VOLATILE

dnl Checks types
AC_CHECK_TYPES([uint32_t  uintptr_t])

dnl Checks functions
AC_CHECK_FUNCS(strcmp sleep)


dnl config.h 
dnl AC_DEFINE define a config.h marco
dnl config.h -> define ENABLE_DEBUG 
dnl AC_DEFINE([ENABLE_DEBUG], 1, [<comment-string>])
if test x"${ENABLE_DEBUG}" = xyes; then
    AC_DEFINE([ENABLE_DEBUG], 1,
              [Define if you want the debug output support compiled in.])
fi

## config M4 使用 [] 代替 `'
## AC_DEFUN 代替 m4_define




## 输出所有的申明文件
AC_OUTPUT

## AC_ARG_ENABLE 添加可选features
AC_ARG_ENABLE(feature, help-string,
              [action-if-given],
              [action-if-not-give])



## e.g. add debug
AC_ARG_ENABLE(debug,
              [  --disable-debug   disable support for debugging output], #help-string
              ENABLE_DEBUG=$enableval,
              ENABLE_DEBUG=yes)
#就是在 ./configure --xxx 如果不制定--disable-dubug 那么执行action-if-not-give
#                         如果制定了 执行action-if-give

## AC_ARG_WITH

## 打印消息
# AC_MSG_NOTICE([xxxx])
# AC_MSG_WARN([xxxx])
# AC_MSG_ERROR([xxx])
# 都会显示一个消息

## AC_CONFIG_MACRO_DIR([])

## 检测依赖库
 AM_PROG_RANLIB # check ranlib
# AM_PROG_LEX


# 制定一个给本项目所有M4宏的一个子目录名
# 生成的aclocal.m4 文件会包含所有在给定子目录下的.m4文件
# 这样做的原因是会自动添加一些宏，而不用我们手动添加
AC_CONFIG_MACRO_DIR([m4])
# 

