/*
 *  Description: tuple 的使用
 *  
 * EE tuple实现，内存布局
 */

//defined in "tuple" header file
#include <tuple>
#include <string>
#include <iostream>
#include <vector>

using namespace std;
struct SS {
  SS(int a=0, int b=0) {}
  int a;
  int b;
};

int main() {
  //EE std::make_tuple 构造一个tuple
  tuple<int, std::string> t1 = std::make_tuple(1,"ofoe");

  //可以存放任何类型
  tuple<std::vector<SS>, int, std::vector<std::string>> ss({1,2,3}, 3, {"oo","ooo"});


	tuple<int, double, string> tt(10, 0.33, "hello");

	//non-member function
	//get<index>()
	std::cout << std::get<0>(tt) << " " ;
	std::cout << std::get<1>(tt) << "\n";

  // std::tuple_element(Index, Tuple)
  // std::tuple_size
  
  //EE 使用std::tie 可以用来解包一个tupel, 类似Erlang中的解构
  //tie 构建一个tuple每个元素都是引用类型,所以才可以解构
  //e.g.
  //int a;
  //std::tuple<int&>{a} = std::tuple<int>{32};
  //等价于
  //std::tie(a) = std::tuple<int>{32};
  //EE std::ignore 用来忽略其他匹配，类似Erlang 中的 {_,_, X} = {1,2,3}
  std::string y;
  std::tie(std::ignore, std::ignore, y) = tt; 
  std::cout << "y:" << y << std::endl;



  //EE std::tuple_cat 拼接多个tuple 到一个
  {
    auto t1 = std::make_tuple(1,2);
    auto t2 = std::make_tuple("oo", 3);

    auto t3 = std::tuple_cat(t1,t2);

  }


  ///内存分布
  printf("sizeof std::tuple<int>            %lu\n", sizeof(std::tuple<int>              ));
  printf("sizeof std::tuple<int,int>        %lu\n", sizeof(std::tuple<int,int>          ));
  printf("sizeof std::tuple<int,int,int>    %lu\n", sizeof(std::tuple<int,int,int>      ));
  printf("sizeof std::tuple<int,double>     %lu\n", sizeof(std::tuple<int,double>       ));
  printf("sizeof std::tuple<double,double>  %lu\n", sizeof(std::tuple<double,double>    ));
  printf("sizeof std::tuple<char>           %lu\n", sizeof(std::tuple<char>             ));

	return 0 ;
}
