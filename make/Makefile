#for git command

CXX_FLAGS=$(shell pkg-config --libs opencv)
## 执行shell 命令
COMMIT_MSG=$(m)
commit:
	git commit -a -m "$(COMMIT_MSG)"

push:
	git push origin master

st:
	git status

.PHONY: commit push all st

## Qus.
# 如何自动添加新的文件?
# 自动生成依赖?
#  使用gcc -M 。。。
#  -M 输出依赖文件，会输出系统文件
#  -MM 和-M相同。但是不会输出系统文件 一般使用-MM 因为系统文件几乎不会变
#  -MD 把-M 的输出写入 .d 文件
#  -MMD 把-MM 的输出写入 .d 文件
# 如何把obj生成到一个指定文件?
#
#EE 默认会执行第一个目标
all: 
	#cmd 必须以tab开始
	echo "all"

# EE 如果有多个目标的依赖一样可以写在一起 e.g aa bb cc
aa bb cc:
	echo "a b c"

# EE  make variable 和 bash 的相似 $var or $(vaar) or ${var}
# make 同佩服和bash的一样, 在目标 和 依赖中的同佩服是有
# make 扩展的，cmd中的由shell扩展。
#
# EE 伪目标：不是以任何实际文件最为的目标。
# e.g all clear 他们是逻辑上的目标，代表一项操作。
# 所有没有参数的make 的目标都是all.
#
# EE .PHONY 特殊目标指示符
# 如果有一个和伪目标同名的文件，那么伪目标就不会被执行，
# buaces 由于没有依赖，make会认为目标已经更新，所以就
# 不会执行相关命令。为了避免这个问题，可以使用.PHONY
# 指式一个目标不是真正的文件目标。
# e.g 可以在最后声明
.PHONY: clean all aa bb cc oo
# 任何伪目标都是没有更新的。这就会在每次make式都执行
# makefile: clean
clean: 
	echo "clean"

# EE 伪目标可以以任何目标为依赖
# e.g
show_msg: build_msg
	echo $(HOME)

build_msg: 
	#EE @是不用回现命令
	@printf "\nBuilding \n"
.PHONY: build_msg show_msg

# EE 一些标准的伪目标
# all
# install
# clean
# distclean
# TAGS
# check

# EE 空目标 目标为一个空文件
# 只记录时间

# EE 自动变量 make 根据上下文自动填充的变量都以$开头
# 只能用在cmd
# $@ 目标名
# $< 第一个依赖条件的文件名
# @? 时间戳在目标时间之后的所以依赖文件，用空格隔开
# @^ 所有依赖文件的名称列表，用空格隔开(不包含重复名词)
# @+ 所有依赖文件的名称列表，用空格隔开(包含重复名词)
# @* 目标的主文件名
# @% 文档成员结构中的文件名
#e.g
autovar: build_msg 
	@echo $@
	@echo $<
	@echo $?

# EE $(@D) $(@F)...
# 上面的符号都可加入D 或 F。D 返回目录部分，F返回文件名
# 可以和上面的结合

nosuffex: s1.c s2.c s3.c
	@echo nosuffex file $(^D) $(^F)


# EE 依赖的莫表将依次执行 
seqence: s1 s2
	@echo seqence depend s1 and s2

.PHONY: seqence s1 s2
s1:
	@echo s1 cmd
s2:
	@echo s2 cmd


## EE vpath
# 在指定的目录中查找文件
# vpath pattern directory-list
# 这样就不用在gcc 中写出file的子目录
vpath %.c %.h src # %相当与bash中的* 在myfile目录中查找myfile后缀文件
mf: a.myfile
	@cat $<

## EE buit-in rule 内置规则
## 查看make模式规则使用,内置变量等 make --print-data-base or -p
## 当没有明确规矩时，就是用隐含规则
## built-in 就是内置规则 ## defualt 是默认规则
# 描述了一个.c 文件编译一个.o文件
# %.o: %.c  # commands to execute (built-in):
#	$(COMPILE.c) $(OUTPUT_OPTION) $<
# 
# 从.c文件产生不具扩展名的文件（执行文件）
# %: %.c #  commands to execute (built-in):
#	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@

# EE 删除内置规则
# 不指定cmd的模式规则会删除make中的相应模式
# e.g.
# %.o：%.c

## EE make 变量 
#和bash 变量相似。只不过赋值时可有有空白符
#必须用括号括主变量名，要不然默认只会识别
#区分大小写
#1个字符的变量。e.g. $ab just $a
#不能用“”括主, 右边的空白会保留
CPPFLAGS = -I include #预处理变量
cppflags:
	@echo $(CPPFLAGS) $CPPFLAGS 
#CC CXX  c/c++ compiler
#CFLAGS CXXFLAGS comiler flags
#CPPFLAGS preprocessor flags
#LDFLAGS linker flags
## EE 变量类型
#两种类型：
#1。简单扩展
#   := 定义简单扩展变量。e.g.
#   := 右边部分会被立即扩展，如果变量没定义，就不和别扩展
MM := a$(CC) -M
NN := a$(BB) -M
simply-variable:
	echo $(MM)
	echo $(NN)

#2. 递归扩展
#  = 定义递归扩展变量。
#  make 会读入=右边的值给变量，并不会立即扩增，
#  知道使用这个变量时才会扩增，也叫惰性扩增变量 e.g.
LL = a$(BB) -M
# 即使在定义LL时BB没有定义但在LL也会被扩展成 abb -M
lzaily:
	echo $(LL)
BB = date 

#EE 3.条件赋值 ?= 只会在变量的值不存在的情况下进行赋值
# ?= 定义的变量会在使用时在扩展
CNN ?= cnn$(CON) 
CON ?= aa
CON ?= bb 

conditional:
	echo $(CON)
	echo $(CNN)

#EE 4.添加赋值 +=
#会跟就+=变量的类型，来就地合适扩展

## EE patten
# make 中 模式是指有%的字符串。%代表任意多个字符。
# 可以放在模式中的任意位置。但只能出现1次。

## EE 静态模式规则 （static pattern rule)
# 这项规则只能用于指定变量
OBJECTS=s1.cc s3.cc #%.cc 会匹配OBJECTS 然后用%.c代替
$(OBJECTS): %.cc: %.c
	@cp $^ $@

static: $(OBJECTS)
	@echo static pattern

## EE suffix rule 后缀模式（旧的make模式）
# .c.o 目标在后 相当与
# %.o %.c

## EE Macor 宏
#使用 define指令创建,define 后是宏的名字，以endef结束
#endef必须独占一行
#e.g.
WALL = -Wall
define compiler-and-run
  @echo $@
  gcc $(WALL) $< -o $@
  #./$@
endef
#Macor 的使用和变量相同
#test1: test1.c
#	@$(compiler-and-run)
#如果在宏的引用前加上@则宏的命令将不会显示

#宏的参数, ??? 可以传参数不？
# 不是 $1 ,,, 
define bb
echo arg is "$1" ; 
endef


## EE 工作目标专属变量,需要特殊处理某一目标，但由于其他
#目标命令行保持一致
#只会印象某一工作目标，对用一些特殊的工作目标来需要用到
#.e.g 比如 test1.c test2.c 可以用一个编译命令，但是test2.c 
#需要使用-O2 选项，这时就可用到，专属变量
tt: test1 test2
test1: test1.c
	$(compiler-and-run)

test2: WALL += -O2  # WALL 离开test2后又会变成原来的形式
test2: test2.c
	$(compiler-and-run)

#工作目标专属变量语法
#target...: var = value
#target...: var := value
#target...: var += value
#target...: var ?= value

# EE 从命令行传入变量
# make CC=gcc CPPFLAGS=`-DBSD -DDEBUG'
# 命令行的变量会覆盖环境中或makefile中的值
# EE 使用override 指令强制使用makefile中的值
# EE 环境变量 当make启动式所有的环境变量都被自动
# 定义为make变量

# EE 导出变量
# 使用export variable....
# 导出所以变量使用export
exp:
	export BB 
# TODO ???倒不出去？？
# 使用unexport 阻止导出，和bash中的export unset 相同。

# EE 条件指令
# ifdef, ifndef, + variable
# ifeq, ifneq  + test
# else endif
# 条件的变量不应用$()括主变量
# test 的形式是 "a" "b" or (a, b) or 'a' 'b'
# (a, b) 形式的test会忽略掉，后的空白
condit:
	@echo conditions strat
ifdef BB
	@echo bb defined
else
	@echo bb not defined
endif
# 注意条件指令不能是tab开头要不会被当作cmd

# EE include
# 引入其他文件
# 当引入的文件不存在时，make会输出一个信息，
# 会继续读取其余的makefile，当所以读完完成
# 后，make会试着从规则中找到一个生成include文件
# 的规则，如果有，就执行该规则，然后重新读入makefile
# 如果没有，就会报错并退出
# 生成依赖文件会使用
# include foo.mk
# $(warning finished include)
#
# foo.mk: bar.mk
# 	m4 --define=FILENAME=$@ bar.mk > @$@

# EE 标准make 变量
# make 内置变量
# MAKE_VERSION 版本
# CURDIR  当前目录
insted:
	@echo $(MAKE_VERSION)
	@echo $(CURDIR)

# EE waring
# 输出性息
.PHONY: warning
warning:
	$(warning this is warning msg)


#Is clone prj remote not orignal?


#EE 进入一个sub descri 执行make
# -C 是进入build 目录 
# -f 制定makefile的名称
#  --no-.. make时不会输出 dir change 信息
#@${MAKE} --no-print-directory -C build -f Makefile

## EE make function 
# make 中有一些内之的函数, 调用这些函数 和读取变量相同需要一$ 开始
#  var = $(funcName arg1, arg2, ...)
# 一些内置函数
# * notdir 出去所有的目录信息, 比如 ../xx/aa 之会有 aa
# SRC=$(notdir)
nodir.o: /var/log/syslog
	@echo $(notdir $<)

## wildcard 搜索指定目录下的文件, 返回以空格间隔的文件名列表,子目录下的文件名会包含子目录
SRCS = $(wildcard *.c ./kernel_ds/*.c ./conf_parser/*.c ./inet/*.c ./time/*.c)

#EE subst [built-in func]
#替换
#e.g 把$(SRCS) 中的.c 替换为 .d
# * 
sub: 
	@echo $(subst .c,.d,$(SRCS))
## shell cmd
#make 中要让变量等于一个shell命令的结果可以
#1. var = `cmd`
#2. var =$(shell cmd)
#对于shell 中的 $(cmd) make 无法得到cmd的结果

ldlibs:
	echo $(LDLIBS) #nil
	echo $(LEXT) #nil
	echo $(OBJEXT) #nil
