/**
 * @file     main.cpp
 *
 */

#include <cctype>
#include <cstdio>
#include <map>
#include <memory>
#include <string>
#include <vector>


#include "lexer.hpp"
#include "parser.hpp"


//===----------------------------------------------------------------------===//
// Main driver code.
// lexer 词法解析(token) -> parser(语法解析) ->  生成ast -> 转换到LLVM IR中间表示语言
//===----------------------------------------------------------------------===//

int main() {
  initBinOpPrec(); //初始化二元操作符优先级

  // Prime the first token.
  fprintf(stderr, "ready> ");
  getNextToken();

  // Run the main "interpreter loop" now.
  MainLoop();

  return 0;
}
