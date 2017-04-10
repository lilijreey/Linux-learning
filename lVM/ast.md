AST 
==================
抽象语法树

AST node 按子节点个数划分
* 点节点(无子节点)
   标识符， 字面值
   
* 二元操作符 节点
   可以表示的有
   +,-,  =, &&, ||, !, ==
   if (test)
       body

* 三元操作符号
  if (test)
      true-exp
  else
      else-exp

 
* [3,4,5,6]
[:var, :array, ]

在token 以后的到的源程序等价的数据结构形式
一般用来，进行语义检测或者优化或者元编程

使用树的形式，可以很好的表达，操作符号的优先级，结合性，和程序流程


AST 只是一种数据结构，并不是可读写的语言。 但是可以把ast,打印出来，然后也可以
按照AST的格式设计AST语言. 直接用AST的形式书写


自定义AST iAST

* 变量
[:var, <type>, <name>, <value>]
type: var, literal, int, string, obj,

* 赋值
[:assign, <=left>, <=right>]
a=3
[:assign, [:var, var, a, nil],
          [:var, literal, nil, 3]]

a=b=3
[:assign, [:var, var, a, nil],
          [:assign, [:var, var, b, nil],
                    [:var, literal, nil, 3]]]

* 四则运算
[:add, <l-exp>, <r-exp>]
[:sub, <l-exp>, <r-exp>]
[:mul, <l-exp>, <r-exp>]
[:div, <l-exp>, <r-exp>]

* 比较
l-exp > r-exp 
[:gt , <l-exp>, <r-exp>]
:lt
:eq
:neq


* 执行控制
if test
    aaa
[:if, <test>, <true-exp>]


if test
    ss
else
[:if-else, <test>, <true-exp>, <else-exp>]

* 函数调用
aa(<args>)

[:call, <name>, <args>]

* 函数定义
aa(args) 
 body

[:def_func <name>, <args>, <body>]
