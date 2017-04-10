/**
 * @file     ins.hpp
 *   VM 指令集          
 *
 * @author   lili <lilijreey@gmail.com>
 * @date     03/23/2016 02:59:37 PM
 *
 */

#ifndef  INS_H
#define  INS_H

#include <cstdint>
#include <cstdlib>
#include <cassert>

//指令是变长的
//指令的类型
//1. 空数据指令
// +-----+ 
// | OP  | 1 byte
// +-----+
//
//2. 1数据指令
// +-----+-----+ 
// | OP  | Num |
// +-----+-----+
// 1byte  1byte
//

typedef uint8_t op_type_t ;

struct Ins {
 public:
  enum OP{
    //search OP_HERE
    OP_UNKNOWN, //无效指令
    OP_NOP, //空指令
    OP_ADD, 
    OP_SUB, 
    OP_MUL, 
    OP_DIV,

    //cmp or jmp
    OP_CMP,
    OP_JMP,
    OP_JLE,
    OP_JGE,
    OP_JE,
    OP_JNE,
    

    OP_PUSH, //op data1
    OP_POP,  //op

    OP_HALT, //停机指令 op
    OP_OVERFLOW_CODE, //访问指令越界 op

  };

  enum TYPE {
    INS_OP,
    INS_NUM,
  };

 public:
  Ins() 
      : _type(OP_UNKNOWN)
      , _len(0)
  {}
  Ins(op_type_t type) 
      : _type(type)
      , _len(sizeof(op_type_t))
  {}

  Ins(op_type_t type, int num) 
      : _type(type) 
      , _len(sizeof(op_type_t) + sizeof(int))
  {
    _data1.num = num;
  }

  void to_s() const;


  op_type_t _type;
  union {
    int num;
  } _data1;
  uint8_t _len; //指令长度
};

class ByteCodes{
 public:
  ByteCodes();
  ~ByteCodes();


 public:
  //构造 bytecodes 
  ByteCodes& op_push(int num) {return add_op(Ins::OP_PUSH, num);}
  ByteCodes& op_pop() {return add_op(Ins::OP_POP);}
  ByteCodes& op_add() {return add_op(Ins::OP_ADD);}
  ByteCodes& op_sub() {return add_op(Ins::OP_SUB);}
  ByteCodes& op_mul() {return add_op(Ins::OP_MUL);}
  ByteCodes& op_div() {return add_op(Ins::OP_DIV);}
  ByteCodes& op_cmp() {return add_op(Ins::OP_CMP);}



 public:
  //显示二进制指令集
  void dump() const ;

  //@return  nextIns Bytes. On no Ins 0 is returns. 
  size_t nextIns(size_t offset, Ins &/*out*/) const;

 private:
  ByteCodes& add_op(Ins::OP);
  ByteCodes& add_op(Ins::OP, int data1);

  template <typename T> ByteCodes& push(T *v);
  template <typename T> 
  ByteCodes& push(T v) { 
    //static_assert(sizeof(T) < sizeof(uint64_t));

    if (_bytecodes_capacity - _bytecodes_size < sizeof(T)) {
      _bytecodes_capacity *= 2;
      _bytecodes = (char*)realloc(_bytecodes, _bytecodes_capacity);
      assert(_bytecodes && "malloc failed");
    }

    *(T*)(_bytecodes + _bytecodes_size) = v;
    _bytecodes_size += sizeof(T);

    return *this;
  }
  ByteCodes& push(const char *data, size_t len);



 private:
  char * _bytecodes{}; //AST 编译出来的指令集
  size_t _bytecodes_size{};
  size_t _bytecodes_capacity{};
};


#endif   /* ----- #ifndef INS_H ----- */

