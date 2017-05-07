/*
 * =====================================================================================
 *
 *       Filename:  regex.cpp
 *
 *    Description:  regule 
 *
 *        Created:  12/09/2012 02:59:23 PM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */
#include <regex>
#include <string>
#include <iostream>

//1 basic_regex
//  定义一个正则表达式
//  basic_regex(const charT *patten, flag_type)
//  flag_type 是正则模式使用的标准
//  std::regex_constants:: 下面定义了多种标准
//  正则表达式可以使用char 或 wchar_t

int main() 
{
  //ab|cd
//  std::basic_regex<char> pattern("ab|cd");
  std::regex pattern("ab", std::regex::egrep);
  //text
  std::string text = "abbb";

  //algorithms
  //std::regex_match  //必须要全部匹配
  //std::cout << std::regex_match(text, pattern);

  std::cout << std::regex_search(text, pattern);

  return 0;
}
