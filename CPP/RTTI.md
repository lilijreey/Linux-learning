runtime type information

通过RTTI，能够通过基类的指针或引用来检索其所指对象的实际类型。c++通过下面两个操作符提供RTTI。
就是编译器的信息，在运行期使用

dynamic_cast 使用RTTI
异常使用

* typeid 操作符使用
  typeid操作符的返回结果是名为type_info的标准库类型的对象的引用
    typeid能够获取一个表达式的类型：typeid(e)。
  如果表达式的类型是类类型且至少包含有一个虚函数，
  则typeid操作符返回表达式的动态类型，需要在运行时计算
  否则，typeid操作符返回表达式的静态类型，在编译时就可以计算。

* type_info class
 Stores information about a type.
  obj.name() 返回一个type字符串

  typeid(int).name()
  c++11 有添加了一个 obj.hash_code()
