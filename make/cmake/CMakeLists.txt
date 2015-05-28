cmake_minimum_required(VERSION 2.8)

project (all_three)
## 一个项目可以有多个可执行文件

## if(0) is false
if(0)
	message("if (0) is true")
else()
	message("if (0) is false")
endif()

## if() is false
if()
	message("if () is true")
else()
	message("if () is false")
endif()

## if(COMMAND xx) check the command is can invoked
if(COMMAND /bin/bash)
	message("if(COMMAND bash) can invoked")
else()
	message("if(COMMAND bash) can not invoked")
endif()

## if(DEFINED var) 如果变量被设置 is true
if(DEFINED xx)
	message("if(DEFINED xx) xx is set")
else()
	message("if(DEFINED xx) xx not set")
endif()

## if(EXISTS dirOrfile) 检测文件是否存在
if(EXISTS ../test_1.c)
	message("if(EXISTS test_1.c) file exsits")
else()
	message("if(EXISTS test_1.c) file not exsits")
endif()

if(IS_DIRECTORY ../)
	message("if(IS_DIRECTORY ../) true")
else()
	message("if(IS_DIRECTORY ../) false")
endif()



aux_source_directory(some_code SOME_SRC)
message("AUX_SOURCE_DIRECTOR some_code: " ${SOME_SRC})
# aux_source_directory 不会递回查找子目录，而且只会返回.c 文件

##　file命令支持递回查找
#file(GLOB variable path) path 支持*
# file返回的是所有和模式匹配的文件不管是什么文件
#file(GLOB FILE_SRC some_code/*)
#message("file(GLOB FILE_SRC: " ${FILE_SRC})

## 递回查找
file(GLOB_RECURSE FILE_SRC some_code/*.h some_code/*.c)
message("file(GLOB_RECURSE FILE_SRC: " ${FILE_SRC})

##  还有很多命令
## EE 如果使用了　aux_source_directory 和file , 当源文件改变时ｃｍａｋｅ
## 使无法得知的，需要重新运行cmake

set(TEST_1_SRC
	test_1.c
)

set(TEST_2_SRC
	test_2.c
)
set(TEST_3_SRC
	test_3.c
)

set(BUILD_TYPE Debug)
## 在使用 cmake 的时候可以cmake -DBUILD_TYPE=xxx 来定义

## 添加第三方库的搜索路径
include_directories(third_libs)

## 添加连接文件搜索路径
# 一般都不用，可用find_library() 来返回这个库的绝对地址
#link_directories(third_libs/other1/libs)


# 编译可执行文件 dd the 3 executable file
add_executable(test_1 ${TEST_1_SRC} )
## 赋给OTHER_LIB 这个变量了
find_library(OTHER_LIB other_lib third_libs/other1/libs)
message("OTHER LIB" ${OTHER_LIB})
## find_library(TCL_LIB
##              NAMES tcl tcl84 tcl83
##		PATHS /usr/lib /user/local/lib)
target_link_libraries(test_1 ${OTHER_LIB})

## find_path 用于查找头文件
find_path(GLWF_INCLUDE_DIR GLFW/glfw3.h /usr/local/include /usr/include)
if (GLWF_INCLUDE_DIR)
	message("GLWF_INCLUDE_DIR is " ${GLWF_INCLUDE_DIR}/GLFW/glfw3.h)
endif()


add_executable(test_2 ${TEST_2_SRC})

## 添加依赖，　test_2 目标依赖lib1 目标
add_dependencies(test_2 lib1) ## ?? 什么作用??
## 给指定的可执行文件设置连接参数
## 设置可以在lib1 定以前就设置
## 对于系统连接库格式和ld 的相同 -lm -lX11
target_link_libraries(test_2 lib1)

add_executable(test_3 ${TEST_3_SRC})
## -lm
target_link_libraries(test_3 -lm)


set(LIB_1_SRC lib.c)
add_library(lib1 SHARED ${LIB_1_SRC})

## 编译库文件 lib
# ADD_LIBRARY(name, SHARED, SOURCES)

## 和make 相同cmake 也支持递归的配置格式
## 如一个sub目录下是一个单独的模块，或者是可执行文件
##　择可以　ADD_SUBDIRECTORY(dir)
## dir 下必须有CMakeLists.txt 文件

##EE 对于第三方库
# 使用　INCLUDE_DIRECTORIES 制定搜索路径　
## FIND_LIBRARY

## 寻找已经安装的包
## FIND_PACKAGE

##EE 平台检测
## check_type_size(
## check_big_endian(

## 设置版本号
set_target_properties(<Target> PROPERTIES VERSION 1.2)

## EE 定义自己的finder
