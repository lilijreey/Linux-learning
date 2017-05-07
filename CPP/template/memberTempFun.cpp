/*
 * =====================================================================================
 *
 *       Filename:  memberTempFun.cpp
 *
 *    Description:  成员模板函数 （类型模板，非类型模板)
 *                 ??如何调用非类型模板构造汉数???
 *
 *        Created:  03/17/2012 10:58:44 AM
 *       Compiler:  gcc4.4
 *
 *         Author:  first name surname (), lilijreey@Gmail.com
 *
 * =====================================================================================
 */

#include	<iostream>
#include	<typeinfo>

enum proc {
	AT, 
	BT, 
};

struct St {
	template <int x>
		St(int s){ i=x;}
	//the same size of follow syntan of elf file
	//template <typename x, x index>
	template<int index>
	int show(){ return a[index];}
	void ss(){std::cout <<"SS" <<std::endl ; }

	int a[100];
	int i;
};

int main(int argc, char *argv[]) 
{

	St x();

	return 0 ;
}

