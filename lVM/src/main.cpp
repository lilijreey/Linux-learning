/**
 * @file     test.cpp
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     03/22/2016 09:11:37 AM
 *
 */

#include <stdlib.h>

#include "ins.hpp"
#include "vm.hpp"
void test1() {
  //test 1 + 1
  
  ByteCodes ins;
  ins.op_push(1)
     .op_push(1111)
     .op_add();
  ins.dump();

  VM vm;
  vm.load(&ins);
  vm.run();
  vm.showInfo();
}

void test2() {
  //cmp
  ByteCodes ins;
  ins.op_push(1)
     .op_push(1111)
     .op_cmp();
  ins.dump();

  VM vm;
  vm.load(&ins);
  vm.run();
  vm.showInfo();
}

int main() {
  test1();
  test2();
  return 0;
}

