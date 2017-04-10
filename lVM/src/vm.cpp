/**
 * @file     vm.cpp
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     03/22/2016 09:56:10 AM
 *
 */
#include <cstdio>
#include "ins.hpp"
#include "vm.hpp"


void VM::load(const ByteCodes *codes)  {
  _rIP = 0;
  _stack.empty();
  _codes = codes;
}


void VM::dumpStack() {
  printf("+======= Stack ========+\n");
  int i=0;
  for (auto e : _stack)  {
    printf("|%3d|\t%d|\n", i++, e);
  }

  printf("+----------------------+\n");
}

void VM::showInfo() {
  printf("==============VM INFO===========================\n");
  printf("IP Reginst: %lu\n", _rIP);
  printf("CMP Reginst: %02x\n", _rCMP);
  dumpStack();
}

void VM::run() {
  if (_state == ST_RUNING) {
    printf("error VM is runing\n");
    return;
  }

  _state = ST_RUNING;
  while (_state == ST_RUNING) {
    Ins ins;
    size_t len = _codes->nextIns(_rIP, ins);
    if (len == 0) {
      //run code finished
      _state = ST_STOP;
      printf("run code finished\n");
      return;
    }
    _rIP += len;
    exec_op(ins);
  }
}


void VM::exec_op(const Ins &ins) {
  printf("run op: ");
  ins.to_s();
  int d1;
  int d2;
  //int d3;
  //int d4;

  //search OP_HERE
  switch (ins._type) {
  case Ins::OP_ADD:
    d1 = stack_pop();
    _stack.back() += d1;
    break;
  case Ins::OP_SUB:
    d1 = stack_pop();
    _stack.back() -= d1;
    break;

  case Ins::OP_MUL:
    d1 = stack_pop();
    _stack.back() *= d1;
    break;

  case Ins::OP_DIV:
    d1 = stack_pop();
    _stack.back() /= d1;
    break;

  case Ins::OP_POP:
    _stack.pop_back();
    break;

  case Ins::OP_HALT:
    _state = ST_STOP;
    break;

  case Ins::OP_PUSH:
    _stack.push_back(ins._data1.num);
    break;

  case Ins::OP_CMP:
    d2 = stack_pop();
    d1 = stack_pop();
    _rCMP = 0;
    if (d1 > d2)
      _rCMP |= CMP_GT;
    else if(d1 < d2)
      _rCMP |= CMP_LT;
    else
      _rCMP |= CMP_EQ;
    break;

  default:
    printf("unknown op %d\n", ins._type);
  };
}

