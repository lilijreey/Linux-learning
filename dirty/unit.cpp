/*
 * =====================================================================================
 *
 *       Filename:  unit.c
 *
 *    Description:  
 *
 *        Created:  03/12/2013 10:15:25 AM
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
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>

std::list<std::string> g_tempDirtyTable;

class nTree
{
 public:
  typedef std::unordered_map<std::string, nTree*>::iterator       iterator;
  typedef std::unordered_map<std::string, nTree*>::const_iterator const_iterator;
  typedef std::unordered_map<std::string, nTree*>::value_type     value_type;
  typedef std::unordered_map<std::string, nTree*>::key_type       key_type;
  typedef std::unordered_map<std::string, nTree*>::mapped_type    T;

  ~nTree()
  {
    for (auto &pair : _t) {
      if (pair.second != nullptr) {
        delete pair.second;
        pair.second = nullptr;
      }
    }
    _t.clear();
  }

  std::pair<iterator, bool> insert(const key_type &key)
  {
    return _t.insert(std::make_pair(key, nullptr));
  }

  std::pair<iterator, bool> insert(const value_type &value)
  {
    return _t.insert(value);
  }

  iterator find(const key_type &key) {
    return _t.find(key);
  }

  const_iterator find(const key_type &key) const {
    return _t.find(key);
  }

  T& operator[](const key_type &key) {
    return _t[key];
  }

  T& operator[](key_type &&key) {
    return _t[key];
  }

  bool empty() const {
    return _t.empty();
  }

  size_t size() const {
    return _t.size();
  }

  iterator begin() {
    return _t.begin();
  }

  const_iterator begin() const {
    return _t.cbegin();
  }

  const_iterator cbegin() const {
    return _t.cbegin();
  }


  iterator end() {
    return _t.end();
  }

  const_iterator cend() const {
    return _t.cend();
  }

  const_iterator end() const {
    return _t.cend();
  }

 private:
  std::unordered_map<std::string, nTree*> _t;
};

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

    //printf("Retrieved line of length %zu :\n", read);
    //printf("%s", line);
    g_tempDirtyTable.push_back(line);
  }


  free(line);
  fclose(fp);
}

//@brief: 得到一个以utf8编码的string中字符的个数
long get_utf8_word_count_n(const unsigned char *buf, size_t len);

//@brief: c风格的字符串
inline
long get_utf8_word_count(const unsigned char *str)
{
  return get_utf8_word_count_n(str, strlen((const char*)str));
}

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

void test_ascii()
{
  char buf[]="12,3";
  assert(is_ascii('a'));
  assert(is_ascii(' '));
  assert(is_ascii('b'));
  assert(is_ascii(buf[1]));
  assert(is_ascii(buf[2]));
  assert(is_ascii(buf[3]));
  assert(is_ascii(&buf[1]));
  assert(is_ascii(&buf[2]));
  assert(is_ascii(&buf[3]));


  assert(!is_ascii("你"));
  assert(!is_ascii("我"));
  assert(!is_ascii("看"));

}


//中文和英文的处理方式不同
//中文一个字是一个word
//从一个字母开始只到空白字符
//word 是存放下一个子的空间的指针， 如果为NULL 由get_next_utf8_word 提供buf
//传给word 
//返回下一个有效word的index
size_t
get_next_utf8_word(const unsigned char *buf, unsigned char **word)
{
  size_t index = 0;
  static unsigned char _word[7] ;
  unsigned char *out = _word;

  if (*word == nullptr) 
    *word = _word;
  else 
    out = *word;

  if (buf[index] >> 7 == 0x00) {  //asiic _1B_WORD 
    //0xxx xxx

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
    assert(0 && "bad");
    return -1;
  }

  memcpy(out, buf, index);
  out[index] = '\0';
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
    if (buf[index] <= 0x7F) {
      //0xxx xxxx
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
  if (index == len) 
    return;

  std::string stw = (char*)word;
  printf("B:%s: |Str:%s",stw.c_str(), line.c_str());
  auto & child = (*ntree)[stw];
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



int main() 
{
  //test_ascii();
      load_dirty_confg("dirty_table.conf");
  ////  showTempTable();
  //
      buildBTable();


  return 0;

}


#if 0
int main() 
{
  char str[] = 
      "wUTF = UCS transformation format UCS 转换格式 \
      !@#$%^&*()！@#￥%……&*（）\
      将Unicode编码和计算机实际编码对应起来的一个规则 \
      UTF-8就是以8位为单元对UCS进行编码。从UCS-2到UTF-8的编码方式如下：\
      对于UTF-8来说最重要的是 每个字节的前两位 \
      UCS-2编码(16进制) 	UTF-8 字节流(二进制) \
      0000 - 007F 	0xxxxxxx \
      0080 - 07FF 	110xxxxx 10xxxxxx          B0 80 以B或B以上开头";

  //EE test get_utf8_word_count
  int i=0;
  for (; i < 100000; ++i)
    get_utf8_word_count((unsigned char*) str);
  //printf("%ld\n", get_utf8_word_count((unsigned char*)str));

  //    EE test get_next_utf8_word
  //    size_t index=0;
  //    size_t len = strlen(str);
  //    while (index < len) {
  //       index += get_next_utf8_word((unsigned char*)str+index);
  //    }
  return 0;
}

#endif
