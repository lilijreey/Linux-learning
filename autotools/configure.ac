# The simplest possible configure.ac has just two line.
# 这些宏都定义在autoconf/*.m4 文件中

# 需要的autoconf 最低版本
AC_PREREQ([2.68])

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

## 检查 C 编译器
AC_PROG_CC

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

## config M4 使用 [] 代替 `'
## AC_DEFUN 代替 m4_define

## 输出所有的申明文件
AC_OUTPUT
