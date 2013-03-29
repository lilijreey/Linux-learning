/*
 * =====================================================================================
 *
 *       Filename:  utf8_encode.h
 *
 *    Description:  utf8 相关
 *
 * =====================================================================================
 */

#ifndef  UTF8_ENCODE_INC
#define  UTF8_ENCODE_INC

#include <string>
#include <string.h>
//#include <stdlib.h>

//isascii in ctype.h
//inline bool is_ascii(unsigned char c)
//{
//  return (unsigned char)c  >> 7 == 0x00;
//}
//
//inline bool is_ascii(char c)
//{
//  return is_ascii((unsigned char)c);
//}



//@brief: 得到utf8 编码的str的字符数
//@parma: str c风格的字符串
//static inline
//long get_utf8_word_count(const unsigned char *str)
//{
//  return get_utf8_word_count_n(str, strlen((const char*)str));
//}

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
    }
    ++words;
  }

  //    printf("index:%lu len:%lu\n",index, len);
  return words;
}

//@brief: 得到下一个字.
//        英文以非数字非字母的符合间隔
//        e.g. hello word 两个字
//        其他utf8 字符每个字符是一个字;
//
//@parma: msg  [IN] utf8字符串
//@parma: word [OUT] 
//        用来存放下一个word. 如果word 传入NULL
//        则由get_next_utf8_word 分配buf。 注意是不可重入的
//        如果word 返回NULL 说明没有找到有效字
//
//@return:返回msg 中wrod的下一个字符的index


//inline size_t
//get_next_utf8_word(const std::string &str, unsigned char **word)
//{
//  return get_next_utf8_word((const unsigned char*)(str.c_str()), word);
//}

size_t
get_next_utf8_word(const unsigned char *msg, unsigned char **word)
{
//  const unsigned char *msg = (const unsigned char*) str;
  size_t index = 0;
  size_t begin = 0;
  static unsigned char _word[32] ;
  unsigned char *out = _word;

  if (*word == nullptr) 
    *word = _word;
  else 
    out = *word;

  if (msg[index] == '\0') {
//    assert(false && "msgis nul str");
    *word = nullptr;
    return index;
  }

check:
  ///// ascii 码开头
  if (msg[index] >> 7 == 0x00) {  //asiic _1B_WORD 
    //0xxx xxx
    //去掉开头都空白字符
    while (isspace(msg[index])) {
      ++index;
      begin = index;
      if (msg[index] == '\0') {
//        printf("没有有效字符 index=%lu |str:%s\n", index,msg);
        *word = nullptr;
        return index;
      }
    }

    //不是英文字符
    if (!isascii(msg[index]))
        goto check;

    //begin word的第一个有效字母
    //英文字符特殊对待： 以空格区分一个英文word，
    ++index;
    while (msg[index] != '\0' &&
           isascii(msg[index]) &&
           !isspace(msg[index]))
    {
       ++index;
    }

    memcpy(out,msg+begin, index);
    out[index-begin] = '\0';
    return index;
  } 


  ////////// 非ascii 吗开头 utf8
  if (msg[index] >> 5 == 0x06) { //_2B_WORD
    //110x xxxx 10xx xxxx
    index += 2;
  } else if (msg[index] >> 4 == 0x0E) { // _3B_WORD
    //1110 xxxx 10xx xxxx
    index += 3;
  } else if (msg[index] >> 3 == 0x1E) { //_4B_WORD
    //1111 0xxx 10xx ...
    index += 4;
  } else if (msg[index] >> 2 == 0x3E) { //_5B_WORD
    //1111 10xx 10xx ...
    index += 5;
  } else if (msg[index] >> 1 == 0x7E) { //_6B_WORD
    //1111 110x 10xx ...
    index += 6;
  } else {
//    assert(0 && "bad");
    *word = nullptr;
    return index+6;
  }

  memcpy(out, msg+begin, index-begin);
  out[index-begin] = '\0';
  return index;
}

#endif   /* ----- #ifndef utf8_encode_INC  ----- */
