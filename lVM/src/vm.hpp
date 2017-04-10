/**
 * @file     vm.hpp
 *           
 * @desc  实现一个VM          
 * VM 有两个重要数据结构,
 * 1. 指令集序列,由AST构造
 * 2. 执行栈, 指令集序列作用的对象
 *
 * @author   lili <lilijreey@gmail.com>
 * @date     03/22/2016 09:12:24 AM
 *
 */

#ifndef  VM_H
#define  VM_H


#include <cstdint>
#include <stack>
#include <vector>


struct Ins;
class ByteCodes;
class VM {
 public:
  VM(int stackSize=20) 
  {
    _stack.reserve(stackSize);
  }

  void run(); //TODO

  void load(const ByteCodes *codes); 

  ///显示当前stack 信息
  void dumpStack();
  void showInfo();

  //debug
  //void next();
  //void stop(); //TODO
  
 private:
  void exec_op(const Ins&);


  inline int stack_pop()  {
    int r = _stack.back();
    _stack.pop_back();
    return r;
  }

 private:
  enum {
    ST_RUNING,
    ST_STOP,
  };


 private:
  uint8_t _state = ST_STOP;

  //registers
  size_t _rIP{}; //指向下一条要执行的_ins 指令的地址
  uint8_t _rCMP{};
  //0 GT
  //1 LT
  //2 EQ

  static const uint8_t CMP_GT = 1;
  static const uint8_t CMP_LT = 1 << 1;
  static const uint8_t CMP_EQ = 1 << 2;

  const ByteCodes * _codes{};
  std::vector<int> _stack;

  friend class ByteCodes;
};

#endif   /* ----- #ifndef VM_H ----- */
