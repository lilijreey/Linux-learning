
struct A
{
    A(int i)
    :_i(i)
    {}

  int operator()()
  {
      
  }
  
  int _i;
};

A a(3);

//Qus. a b 的区别
std::bind(a);
std::bind(&A::operator(), &a);
