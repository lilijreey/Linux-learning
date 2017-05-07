/*
 *  Description: initializer_list 类型，使用他可以用{}初始化非POD class
 *
    可以在变量定义中使用初始化器列表
    可以在 new 表达式中使用初始化器列表
    可以用作函数参数和/或函数返回语句
    允许作为下标表达式
    允许作为构造器调用的参数
    不允许类型窄化 e.g int xx[] = {1.22, 34.33}; error for initializer_list, 是为了避免某些BUG
 */

#include <stdio.h>
#include <vector>

using namespace std;

//this is a POD type class can use initializer-list-constructor
struct POD1{
    int a;
    double d;
    void ss(){};
};

//In C++03 can not use initializer-list-constructor create NonPOD1
struct NonPOD1{

    //C++0x proviet "initializer_list<>", this is a first-class 
    NonPOD1(initializer_list<int> list) : ll(list) {}
    vector<int> ll;
    void ss(){};
};

int main() {
    POD1 p1={1,0.2};
    p1.ss();

    //创建initializer_list 类型
    initializer_list<int> l1= {1,2,3};

    //vector, list, map 等stl容器已经提供了使用
    //initializer-list-constructor,特性的构造函数
    //可以这样写
    //vector<int> oo={1,2,3};
    //map<int,string> decfg
    
    //访问initializer_list 
    for (auto it=l1.begin(); it != l1.end(); ++it) {
	printf ( "%d ", *it );
    }

    auto x1 = {1,2};
    //x1 的类型是initializer_list<int>
    
    
    NonPOD1 p2={1,2,3};
    p2.ss();

    return 0 ;
}


