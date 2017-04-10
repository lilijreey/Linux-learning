
#include "ins.hpp"
#include <cstring>
#include <cstdio>


//TODO 多态解码(工厂函数)
static size_t parseIns(const char *buf, Ins &ins /*out*/) {
  size_t ins_len=1; //op_type
  uint8_t op_type = *(const uint8_t *)buf;
  ins._type = op_type;

  //search OP_HERE
  switch (op_type) {
  case Ins::OP_PUSH:
    ins._data1.num =  *(const int*)(buf+ins_len);
    ins_len += sizeof(int);
    break;

  default: //one op
    break;
  };

  return ins_len;
}

struct OpInfo {
  const char *_name;
};


#define	OP_NAME(name) {#name}

//SERACH_OP
static OpInfo s_op_name[] = {
   OP_NAME(OP_UNKNOWN),
   OP_NAME(OP_NOP), 
   OP_NAME(OP_ADD), 
   OP_NAME(OP_SUB), 
   OP_NAME(OP_MUL), 
   OP_NAME(OP_DIV),
   OP_NAME(OP_CMP),
   OP_NAME(OP_JMP),
   OP_NAME(OP_JLE),
   OP_NAME(OP_JGE),
   OP_NAME(OP_JE),
   OP_NAME(OP_JNE),
   OP_NAME(OP_PUSH), 
   OP_NAME(OP_POP), 
   OP_NAME(OP_HALT),
   OP_NAME(OP_OVERFLOW_CODE), 
};

ByteCodes::ByteCodes() {
  _bytecodes_capacity = 1 << 7;
  _bytecodes = (char *)malloc(_bytecodes_capacity);
  assert(_bytecodes && "malloc failed");
}

ByteCodes::~ByteCodes() {
  free(_bytecodes);
  _bytecodes_size = 0;
  _bytecodes_capacity = 0;
}


void Ins::to_s() const {
  //search OP_HERE
  printf("%s", s_op_name[_type]._name);
  if (_type == OP_PUSH)
    printf(" %d", _data1.num);

  printf("\n");
}

ByteCodes& ByteCodes::add_op(Ins::OP op)  {
  push<uint8_t>(op);
  return *this;
}

ByteCodes& ByteCodes::add_op(Ins::OP op, int data1) {
  push<uint8_t>(op);
  push(data1);
  return *this;
}

//const ByteCodes::Ins& ByteCodes::overflow_code_ins() {
  //static Ins overflow_code(INS_OP, OP_OVERFLOW_CODE);
  //return overflow_code;
//}


void ByteCodes::dump() const {
  Ins ins;
  size_t len;
  size_t offset=0;

  printf("+---------ByteCodes dump----------+\n");
  while (true) {
    len = nextIns(offset, ins);
    if (len == 0)
      break;

    printf("%06lx: ", offset);
    //print Ins binary fomat
    for (size_t i = 0; i < len; ++i) {
      printf("%02x ", *(uint8_t*)(_bytecodes + offset + i));
    }
    printf(" ;");
    ins.to_s();
    offset += len;
  }
  printf("+----------------------------------+\n");
}

size_t ByteCodes::nextIns(size_t offset, Ins &ins) const {
  if (offset >= _bytecodes_size)
    return 0;
  
  return parseIns(_bytecodes+offset, ins);
}

ByteCodes& ByteCodes::push(const char *data, size_t len)
{ 
  if (_bytecodes_capacity - _bytecodes_size < len) {
    _bytecodes_capacity =
        _bytecodes_capacity * 2 > _bytecodes_capacity + len ?
        _bytecodes_capacity * 2 : 
        _bytecodes_capacity + len;

    _bytecodes = (char*)realloc(_bytecodes, _bytecodes_capacity);
    assert(_bytecodes && "malloc failed");
  }

  memcpy(_bytecodes, data, len);
  _bytecodes_size += len;

  return *this;
}



