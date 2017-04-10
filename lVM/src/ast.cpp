/**
 * @file     ast.hpp
 *           
 * @author   lili <lilijreey@gmail.com>
 * @date     03/24/2016 09:46:37 AM
 *
 * see ../ast.md
 */


#include <stdio.h>
#include <string>
//定义AST 数据结构
//使用C++ init-list 书写AST, 转换为对应AST, 并打印AST

enum TAG_TYPE{
  TAG_indent, //标识符
  TAG_assign, //赋值

};

struct Node {
 public:
  virtual ~Node() {};
  virtual void show(int indent=0) const =0;
  virtual TAG_TYPE node_type() const = 0;
  virtual const char *node_type_name() const = 0;
};


struct VarNode : Node {
  static VarNode* makeVar(const std::string &name) {
    VarNode *n = new VarNode();
    n->_type = "var";
    n->_name = name;
    n->_value = "nil";
    return n;
  }

  static VarNode * makeLiteral(const std::string &value) {
    VarNode *n = new VarNode();
    n->_type = "literal";
    n->_name = "nil";
    n->_value = value;
    return n;
  }

  virtual void show(int indent=0) const override {
    printf("%*.*s",indent, indent,"");
    printf("[:indent, ");
    printf("%s, ", _type.c_str());
    printf("%s, ", _name.c_str());
    printf("%s]", _value.c_str());
  }

  virtual TAG_TYPE node_type() const override { return TAG_indent; }
  virtual const char* node_type_name() const override { return "indent"; }

  std::string _type;
  std::string _name;
  std::string _value;
};



void testVar() {
  printf("test Var-------------------------\n");
  VarNode::makeVar("aa")->show(); //aa
  printf("\n");
  VarNode::makeLiteral("3")->show(); //3
  printf("\n");
}


struct TwoOpNode : Node {
  TwoOpNode(Node *l, Node *r) :
      left(l), right(r)
  {}

  virtual void show(int indent=0) const override {
    printf("%*.*s%s%s",indent, indent+10,"[", node_type_name(), ", ");
    left->show();
    printf(",\n");
    right->show(indent+10);
    if (indent == 0) 
      printf("]\n");
    else
      printf("]");
  }

  virtual TAG_TYPE node_type() const override {
    return TAG_assign;
  }
  virtual const char* node_type_name() const override { return ":tow_op_node"; }

  Node *left;
  Node *right;
};

struct AssignNode : TwoOpNode {
  static AssignNode* make(Node *l, Node *r) {
    return new AssignNode(l,r);
  }

  using TwoOpNode::TwoOpNode;
  virtual const char* node_type_name() const override { return ":assign"; }

};

void testAssign() {
  //a=3
  printf("test Assing -------------------------\n");
  AssignNode::make(VarNode::makeVar("a"),
                   VarNode::makeLiteral("3"))->show();

  //a=b=3
  AssignNode::make(VarNode::makeVar("a"),
                   AssignNode::make(VarNode::makeVar("b"),
                                    VarNode::makeLiteral("3")))->show();
}

struct 


int main() {
  testVar();
  testAssign();
  
  return 0;
}
