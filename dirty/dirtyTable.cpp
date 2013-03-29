/*
 * =====================================================================================
 *
 *       Filename:  dirty_check.cpp
 *
 *    Description:  敏感词过滤系统
 *
 *        Created:  03/15/2013 12:00:28 PM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <functional>
#include "dirtyTable.hpp"
#include "utf8_encode.h"

void DirtyTable::buildBTable()
{
  for (const auto &line : _tempDirtyTable) {
    doBuildTalbe(&_dirtyTable, line);
  }
  _tempDirtyTable.clear();
}

bool DirtyTable::loadDirtyTable(const char *filename) 
{
  FILE *fp;
  char *line = nullptr;
  size_t len = 0;
  ssize_t read;

  fp = fopen(filename, "r");
  if (fp == nullptr) {
    perror("fopen error");
    return false;
  }

  while ((read = getline(&line, &len, fp)) != -1) {
    if (line[0] == '#') {
      continue;
    }

    if (isspace(line[0])) {
      continue;
    }

    //## 删除每行的结束标记  unix  CR
    if (line[read-1] == '\n')
      line[read-1] = '\0';

    //printf("Retrieved line of length %zu :\n", read);
    //printf("%s", line);
    _tempDirtyTable.push_back(line);
  }

  free(line);
  fclose(fp);

  return true;
}

void DirtyTable::doBuildTalbe(MultiwayTree *ntree, const std::string &line)
{
  size_t len = line.length();
  size_t index=0;
  unsigned char *word;
  if (len == 0) 
    return;

  word = nullptr;
  index = get_next_utf8_word((unsigned char*)line.c_str(), &word);
  if (word == nullptr) 
    return;

#ifdef DEBUG
  printf("B:%s: |Str:%s\n",word, line.c_str());
#endif
  auto & child = (*ntree)[(char*)word];
  if (index == len) 
    return;

  if (child == nullptr) {
    child = new MultiwayTree();
  }

  return doBuildTalbe(child, line.substr(index));
}

void DirtyTable::filtrateSensitiveWords(char *str, size_t msg_len) 
{
  unsigned char *msg  = (unsigned char*)str;
  if (msg_len == 0) 
    return ;

#define SHOW_MATCH
#ifdef SHOW_MATCH
  char dd[32];
#endif

  size_t index=0, next ; //执行正在被扫描的B
  bool isMatch =false;
  while (index < msg_len) {
    next = doFiltrateSensitiveWord1(&_dirtyTable, msg+index, msg_len - index, isMatch);
    if (isMatch) {
#ifdef SHOW_MATCH
      memcpy(dd, str + index, next);
      dd[next+index] = '\0';
      printf("index:%lu next:%lu :%s\n", index, next, dd);
#endif
      isMatch = false;
    }
    index += next;
  }
}


size_t DirtyTable::doFiltrateSensitiveWord1(MultiwayTree *tree, unsigned char *msg, size_t msg_len, bool &isMatch) 
{
  size_t index=0 ; //执行正在被扫描的B
  unsigned char *word;

  if (msg_len == 0) 
    return 0;

    word = nullptr;
    //需要使用更加智能的分词算法
    //TODO 代替get_next_utf8_word
    index = get_next_utf8_word(msg+index, &word);
    if (word == nullptr) 
      return index;

    //TODO std::basic_string<unsigned char>
#if DEBUG
    printf("check word:%s\n",word);
#endif

    const auto it = tree->find((char*) word);
    if (it == tree->end()) {
      //没有在B表中 下一个字符
      return index;
    }

    //到达底端
    if (it->second == nullptr) {
      isMatch = true;
      return index;
    }

    //试着匹配下一个
    return index + doFiltrateSensitiveWordN(it->second, msg+index, msg_len-index, isMatch);

}


size_t DirtyTable::doFiltrateSensitiveWordN(MultiwayTree *tree, unsigned char *msg, size_t msg_len, bool &isMatch) 
{
  size_t index=0 ; //执行正在被扫描的B
  unsigned char *word;

  if (msg_len == 0) 
    return 0;

    word = nullptr;
    //需要使用更加智能的分词算法
    //TODO 代替get_next_utf8_word
    index = get_next_utf8_word(msg+index, &word);
    if (word == nullptr) 
      return index;


#if DEBUG
    printf("check word:%s\n",word);
#endif

    const auto it = tree->find((char*)word);
    if (it == tree->end()) {
      //没有在B表中 下一个字符
      return 0;
    }

    //到达底端
    if (it->second == nullptr) {
      isMatch = true;
      return index;
    }

    //试着匹配下一个
    return index + doFiltrateSensitiveWordN(it->second, msg+index, msg_len-index, isMatch);
}

