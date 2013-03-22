/*
 * =====================================================================================
 *
 *       Filename:  unit.c
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <functional>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>

//打印调试信息
//#define DEBUG

void nTree::showTable(int level) const
{
  for (auto &pair : _t) {
    printf("key:%s\n", pair.first.c_str());
  }
}

/*first word hash value*/
nTree g_BTable;

void load_dirty_confg(const char *filename) 
{
  FILE *fp;
  char *line = nullptr;
  size_t len = 0;
  ssize_t read;

  fp = fopen(filename, "r");
  if (fp == nullptr) {
    perror("fopen error");
    exit(1);
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
    g_tempDirtyTable.push_back(line);
  }


  free(line);
  fclose(fp);
}

//@brief: 得到一个以utf8编码的string中字符的个数
long get_utf8_word_count_n(const unsigned char *buf, size_t len);


//第一个版本使用右移操作 判断一个字的类型
#if 1
//10W 0.10 sec
long get_utf8_word_count_n(const unsigned char *buf, size_t len)
{
  size_t index = 0;
  long words = 0;

  while (index < len) {
    if (buf[index] >> 7 == 0x00) {//asiic _1B_WORD 
      //0xxx xxxx
      ++index;
    } else if (buf[index] >> 5 == 0x06) { //_2B_WORD
      //110x xxxx 10xx xxxx
      index += 2;
    } else if (buf[index] >> 4 == 0x0E) { // _3B_WORD
      //1110 xxxx 10xx xxxx
      index += 3;
    } else if (buf[index] >> 3 == 0x1E) { //_4B_WORD
      //1111 0xxx 10xx ...
      index += 4;
    } else if (buf[index] >> 2 == 0x3E) { //_5B_WORD
      //1111 10xx 10xx ...
      index += 5;
    } else if (buf[index] >> 1 == 0x7E) { //_6B_WORD
      //1111 110x 10xx ...
      index += 6;
    } else {
      printf("error unkonw:%u\n",buf[index]);
      return -1;
    }
    ++words;
  }

  //    printf("index:%lu len:%lu\n",index, len);
  return words;
}


inline bool is_ascii(unsigned char c)
{
  return (unsigned char)c  >> 7 == 0x00;
}

inline bool is_ascii(char c)
{
  return is_ascii((unsigned char)c);
}

inline bool is_ascii(const unsigned char *c)
{
  return *c  >> 7 == 0x00;
}

inline bool is_ascii(const char *c)
{
  return is_ascii((const unsigned char*)c);
}


//中文和英文的处理方式不同
//中文一个字是一个word
//从一个字母开始只到空白字符
//word 是存放下一个子的空间的指针， 如果为NULL 由get_next_utf8_word 提供buf
//传给word 
//buf: 必须是一个C 风格的str
//如果word返回null说明没有有效字符
//返回下一个有效word的index
size_t
get_next_utf8_word(const unsigned char *buf, unsigned char **word)
{
  size_t index = 0;
  size_t begin = 0;
  static unsigned char _word[32] ;
  unsigned char *out = _word;

  if (*word == nullptr) 
    *word = _word;
  else 
    out = *word;

  if (buf[index] == '\0') {
    assert(false && "buf is nul str");
    *word = nullptr;
    return index;
  }

check:
  ///// ascii 码开头
  if (buf[index] >> 7 == 0x00) {  //asiic _1B_WORD 
    //0xxx xxx
    //去掉开头都空白字符
    while (isspace(buf[index])) {
      ++index;
      begin = index;
      if (buf[index] == '\0') {
        printf("没有有效字符 index=%lu |str:%s\n", index, buf);
        *word = nullptr;
        return index;
      }
    }

    //不是英文字符
    if (!is_ascii(buf[index]))
        goto check;

    //begin word的第一个有效字母
    //英文字符特殊对待： 以空格区分一个英文word，
    ++index;
    while (buf[index] != '\0' &&
           is_ascii(buf[index]) &&
           !isspace(buf[index]))
    {
       ++index;
    }

    memcpy(out, buf+begin, index);
    out[index-begin] = '\0';
    return index;
  } 


  ////////// 非ascii 吗开头 utf8
  if (buf[index] >> 5 == 0x06) { //_2B_WORD
    //110x xxxx 10xx xxxx
    index += 2;
  } else if (buf[index] >> 4 == 0x0E) { // _3B_WORD
    //1110 xxxx 10xx xxxx
    index += 3;
  } else if (buf[index] >> 3 == 0x1E) { //_4B_WORD
    //1111 0xxx 10xx ...
    index += 4;
  } else if (buf[index] >> 2 == 0x3E) { //_5B_WORD
    //1111 10xx 10xx ...
    index += 5;
  } else if (buf[index] >> 1 == 0x7E) { //_6B_WORD
    //1111 110x 10xx ...
    index += 6;
  } else {
    printf("error unkonw:%u\n",buf[index]);
    assert(0 && "bad");
    return -1;
  }

  memcpy(out, buf+begin, index-begin);
  out[index-begin] = '\0';
  //    printf("%s\n", out);
  return index;
}

size_t
get_next_utf8_word(const std::string &str, unsigned char **word)
{
  return get_next_utf8_word((unsigned char*)str.c_str(), word); 
}

#else
//10W 0.10 sec 和第一种算法效率相同
// 第二种分词算法， 使用循环判断首字节1的个数
long get_utf8_word_count_n(const unsigned char *buf, size_t len)
{
  size_t index = 0;
  long words = 0;

  while (index < len) {
    if (buf[index] <= 0x7F) { //A ascii
      //0xxx xxxx
      //
      ++index;
    } else if (buf[index] <= 0xDF  ) { //_2B_WORD
      //110x xxxx 10xx xxxx
      index += 2;
    } else if (buf[index] <= 0xEF) { // _3B_WORD
      //1110 xxxx 10xx xxxx
      index += 3;
    } else if (buf[index] <= 0xF7) { //_4B_WORD
      //1111 0xxx 10xx ...
      index += 4;
    } else if (buf[index] <= 0xFB) { //_5B_WORD
      //1111 10xx 10xx ...
      index += 5;
    } else if (buf[index] <= 0xFD) { //_6B_WORD
      //1111 110x 10xx ...
      index += 6;
    } else {
      printf("error unkonw:%u\n",buf[index]);
      return -1;
    }
    ++words;
  }

  //    printf("index:%lu len:%lu\n",index, len);
  return words;
}

#endif


void showTempTable()
{
  for (const auto & str : g_tempDirtyTable)
    printf("%s",str.c_str());
}


void doBuildTalbe(nTree *ntree, const std::string &line)
{
  size_t len = line.length();
  size_t index=0;
  unsigned char *word;
  if (len == 0) 
    return;

  word = nullptr;
  index = get_next_utf8_word(line, &word);
  if (word == nullptr) 
    return;

  std::string stw = (char*)word;
#ifdef DEBUG
  printf("B:%s: |Str:%s\n",stw.c_str(), line.c_str());
#endif
  auto & child = (*ntree)[stw];
  if (index == len) 
    return;

  if (child == nullptr) {
    child = new nTree();
  }


  return doBuildTalbe(child, line.substr(index));
}

//构造首字table
void buildBTable()
{
  for (const auto & line : g_tempDirtyTable) {
    doBuildTalbe(&g_BTable, line);
  }
}

void showNtree()
{
  g_BTable.showTable();
}


size_t nTree::doFiltrateSensitiveWord1(unsigned char *msg, size_t msg_len) const
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

    std::string stw = (char*)word;
    printf("check word:%s\n",word);

    const auto it = find(stw);
    if (it == end()) {
      //没有在B表中 下一个字符
      return index;
    }

    if (it->second == nullptr) {
      printf("有屁配的\n");
      return index;
    }

    //试着匹配下一个
    return index + it->second->doFiltrateSensitiveWords(msg+index, msg_len-index);

}

void nTree::filtrateSensitiveWords(unsigned char *msg, size_t msg_len) const
{
  if (msg_len == 0) 
    return ;

  size_t index=0 ; //执行正在被扫描的B
  while (index < msg_len) {
    index += doFiltrateSensitiveWord1(msg+index, msg_len - index);
  }
}


size_t nTree::doFiltrateSensitiveWords(unsigned char *msg, size_t msg_len) const
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

    std::string stw = (char*)word;
    printf("check word:%s\n",word);

    const auto it = find(stw);
    if (it == end()) {
      //没有在B表中 下一个字符
      return 0;
    }

    if (it->second == nullptr) {
      printf("有屁配的 dirty\n");
      return index;
    }

    //试着匹配下一个
    return index + it->second->doFiltrateSensitiveWords(msg+index, msg_len-index);
}

//int main() 
//{
//  //test_ascii();
//      load_dirty_confg("dirty_table.conf");
//  //showTempTable();
//      buildBTable();
//  //g_BTable.showTable();
//  
//      char msg[] = "hello 肛交 伞熹捂着耳朵，笑盈盈的跑了出来。“您好！欢迎光临。”她轻轻的点了点头。那黑色的风衣一颤，慢慢的转过身来。天啊！"
//      g_BTable.filtrateSensitiveWords(msg);
//
//
//  return 0;
//
//}
//
