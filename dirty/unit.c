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
            return -1;
        }
        ++words;
    }

//    printf("index:%lu len:%lu\n",index, len);
    return words;
}

long get_next_utf8_word(const unsigned char *buf)
{
    size_t index = 0;
    char _word[7] ;

    if (buf[index] >> 7 == 0x00) {//asiic _1B_WORD 
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

    memcpy(_word, buf, index);
    _word[index] = '\0';
    printf("%s\n", _word);

    return index;
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
