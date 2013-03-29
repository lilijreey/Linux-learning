/*
 * =====================================================================================
 *
 *       Filename:  dirtyTable.h
 *
 *    Description:  
 *
 * =====================================================================================
 */

#ifndef  DIRTYTABLE_INC
#define  DIRTYTABLE_INC

#include <list>
#include <string>
#include <string.h>
#include "multiwayTree.hpp"

//@brief: 敏感词类
class DirtyTable
{
public:
  //@brief: 加载词库
  bool loadDirtyTable(const char *filename);

  //@brief: 构造词库
  void buildBTable();

  //for debug TODO 答应全部 (只打印第1层）
  //void showTable(int level) const {_dirtyTable.showTable();}

  //@brief: 过滤敏感词
  //@parma: msg 需要过滤的文本，
  //@parma: msg的长度 strlen(msg)
  //@return: 返回是否有敏感词
  void filtrateSensitiveWords(char *msg, size_t msg_len);

private:
  void doBuildTalbe(MultiwayTree *ntree, const std::string &line);

  //_dirtyTable 第一层使用
  size_t doFiltrateSensitiveWord1(MultiwayTree *tree, unsigned char *msg, size_t msg_len, bool &isMatch);
  //其它层使用
  size_t doFiltrateSensitiveWordN(MultiwayTree *tree, unsigned char *msg, size_t msg_len, bool &isMatch);

  //debug
//  void showTable(int level=1) const;

 private:
  std::list<std::string> _tempDirtyTable;
  MultiwayTree           _dirtyTable;
};


#endif   /* ----- #ifndef dirtytable_INC  ----- */
