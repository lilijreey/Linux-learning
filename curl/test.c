/**
 * @file     test.c
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     2015年05月28日 09时16分56秒
 *
 * man libcurl-tutorial
 * 
 */
#include <stdio.h>
#include <curl/curl.h>

//EE 编译参数 使用 curl-config --libs 得到
//curl-config --feature

//EE curl 编程结构
//EE curl_global_init() 在使用curl 之前必须先调用
//EE curl_global_cleanup() 在不使用curl 时调用，用来释放 curl_global_init 的资源
//   !!! 上面两个函数都只应该调用一次

//EE curl_version_info

//EE API interface
//* curl 提供种接口， 一种 curl_easy 所有的easy interface 都是synchronous and blcoking fn call
//* multiple interface. allows 多次调用在一个thread上

//Easy  
// to use the easy API, must first create a easy-handle, one thread one easy-handle
// must never share the same handle in multiple threads
//
//EE create easy-handle
//  curl_easy_init();
//  
//EE 设置参数
// curl_easy_setopt()
//   
//EE 清空参数
//  curl_easy_reset
//  
//EE clone handle
//  curl_easy_duphandle
//
//EE set url
//  curl_easy_setopt(handle, CURLOPT_URL, "http://baidu.com/");
//
//EE 得到返回的数据
//  需要自己定义一个函数，curl 会调用你的函数
//	size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);
//       You tell libcurl to pass all data to this function by issuing a function similar to this:
//	curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, write_data);
//	
//EE userp 第四个参数使可以指定的
// You can control what data your callback function gets in the fourth argument by setting	another
//       property:
//	curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &internal_struct);
//
//EE 如果自己不设置WRITEFUNCTION curl 会调用自己的函数，就是把所有的数据输出到stdout 
// 

//EE 执行HTTP 请求 
//  curl_easy_perform(handle)
//  will connect to the remote site.
//  if recive data, it calls the callback fn
//  注意，回调的函数必须返回和传入的参数相同的字节数，如果不一致，curl会中断操作并且返回error-code


// EE HTTP GET
#if 1 

size_t read_data_cb(void *buffer, size_t size, size_t nmemb, void *userp)
{
    printf("read_data_cb size %lu nmemb %lu\n", size, nmemb);
    return nmemb;
}

int main(int argc, char *argv[])
{
  CURL *curl;
  CURLcode res;

  /* In windows, this will init the winsock stuff */
  curl_global_init(CURL_GLOBAL_ALL);

  /* get a curl handle */
  curl = curl_easy_init();

  if (!curl) {
      printf("curl_easy_init error\n");
      goto exit;
  }

  //set URL
  curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com/");
//  curl_easy_setopt(curl, CURLOPT_URL, "http://www.2345.com/");

  //set callback
 curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, read_data_cb);

  /* Perform the request, res will get the return code */
  res = curl_easy_perform(curl);
  printf("curl_easy_perform finished\n");

  /* Check for errors */
  if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

  /* always cleanup */
  curl_easy_cleanup(curl);

exit:

  curl_global_cleanup();
  return 0;
}				/* ----------  end of function main  ---------- */

#endif
// EE HTTP POST

