cmake_minimum_required(VERSION 2.8)

## 第三方库
project (OTHER)
set(OTHER_SRC
	other.h
	other.c
)

## 如何指定编译后库的存放目录
set(LIB_1_SRC lib.c)
add_library(other_lib SHARED ${OTHER_SRC})

