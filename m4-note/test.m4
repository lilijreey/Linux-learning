dnl# define(AA,` lili')dnl
dnl# AA.
dnl# `AA' is AA
dnl
define(LP,`(')dnl
define(RP,`)')dnl
LP...RP

dnl # not echo 
define(`OK',```BRACK''')dnl
OK

LEFT
undefine(`LEFT')dnl
LEFT

dnl #EE 只用DD 调用了num才是一个宏
dnl # 对DD的调用没有输出，只是定义了一个num宏
define(`DD', `define(num, 99)')dnl 
    num
    DD num
    num

dnl # define(`DD', define(num, 99))
dnl # EE 和上面有啥不同。如果一个镶嵌的宏不quoted。 
dnl # 他就会立即执行，就是在define的时候，
dnl # 这时DD就是一个空串

dnl # 多个define嵌套
define(`BB',`define(`B1',`define(`B2',3)')')dnl
B2 #"B2" 
B1 #"B1" 
BB #"BB" 
B1 #"B1" 
B2 #"B2" 

define(`XX', CC)dnl
XX
XX(1, 2, 3)
`XX'

define(new, `$1')
new(XX)

define(foo, `( () ')
foo

define(TEMP)
TEMP

define(`hello m4', `HeiHei')

define(`M5', ``$0:$2$1 argument count is:$#eff'')dnl
M5(ok, hah,wxy)
define(`M6', `$*')dnl
M6(1,2,3,4,5)

define(`M7', `$@')dnl
define(`M8', `this is macro `M7'.')dnl
undefine(`M8')
M6(M8)
M7(M8)
indir(`M7',1,2,3)
indir
ifdef(`M7', ``M7' is  defined', ``M7' is not defined')

define(`DD1', `dd1')
define(`DD3', `dd3')
define(`DD2', ``DD1'')
define(`DDx', `$1': ``DD3'' : `DD2')
#dumpdef(`DD1', `)
#dumpdef(`DDx')

traceon(`DD1',`DDx')
DD1()
DDx()

# arothimir
define(`INC', `incr(`$1')')
INC(1)

format(`Result is %d', eval(2**4))

# empty string define(`xxx', ``xxx'`'ok')
define(`active', `ACC, OOX')
define(`show', `$1 $1')
show(```active''')
`active'

indir(define, `oxo', `1o1')
oxo
ifelse(oxo)

divert(`1')dnl
 `1 divert output'
divert dnl
 `0 divert output'
divert dnl
    `first'
divert dnl
    `second'

