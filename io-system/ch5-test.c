/*
 * =====================================================================================
 *
 *       Filename:  ch5-test.c
 *
 *    Description:  about chapter-5 test std I/O *
 *        Version:  1.0
 *        Created:  10/03/2011 08:13:07 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */


#include	<stdio.h>
#include	<wchar.h> /* fwide */
#include	<errno.h>
#include    <unistd.h>

#include	<zhao_include/myUtility.h>
/*
 * fopen 建立一个文件的默认权限与umask 的关系
 * From manpage: Any created files will have mode 
 *	S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH (0666),
 *	as modified by the  process's  umask  value  (see umask(2)).
 *
 * From umake(2):The typical default value for the process umask is
 * S_IWGRP | S_IWOTH  (octal 022). 
 * In the usual case where the mode argument to open(2) is specified as:
 *       S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
 *       (octal 0666) when creating a new file, the permissions on the resulting
 *       file will be:  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
 *       (because 0666 & ~022 = 0644; i.e., rw-r--r--).
 *
 *  即文件建立时默认的权限为066, 在与umask值 ~&, 就得到文件的实际权限
 *  注意：在别的文件系统中不行，如ntfs，默认为0777，root所有
 */
#if 0
int main() {
	FILE *create_file = fopen("temple-file", "w+") ;
	if (NULL == create_file)
		perror("open file error") ;

	/* 
	 * test stream orientont
	 * 并没要绑定流
	 * 为了建议错误，清空errno
	 */
	errno = 0 ;
	int ori = fwide(create_file, 0) ;
    if (0 != errno)		perror("fwide error") ;
	if (ori > 0) 
		printf("oriented wchar\n") ;
	else if (ori == 0)
		printf("stream undecide\n") ;
	else  
		printf("oriented char\n") ;

	fclose(create_file) ;
	return 0 ;

}
#endif

/*
 * 缓冲：三种类型，1无，2行，3全缓冲。
 * setbuf:设置缓冲 char* buf 设置为 定义位BUFSIZ = 8019, or NULL no buffer
 * 重定向到文件后都为full缓冲
 *
 *The setvbuf() function may be used on any open stream  to  change  its
       buffer.  The mode argument must be one of the following three macros:

              _IONBF unbuffered

              _IOLBF line buffered

              _IOFBF fully buffered

       Except for unbuffered files, the buf argument should point to a buffer
       at least size bytes long; this buffer will be used instead of the cur‐
       rent  buffer.  If the argument buf is NULL, only the mode is affected;
       a new buffer will be allocated on the next read  or  write  operation.
       The  setvbuf()  function  may  only be used after opening a stream and
       before any other operations have been performed on it.

       The other three calls are, in effect,  simply  aliases  for  calls  to
       setvbuf(). 
	The setbuf() function is exactly equivalent to the call
           setvbuf(stream, buf, buf ? _IOFBF : _IONBF, BUFSIZ);
 */

#if 0
int main() {
	fprintf(stderr, "default stderr is nobuffer\n") ;

	char stream_buf[BUFSIZ] ; /* BUFSIZ is 8019 */

	fprintf(stderr, "strderr flushed\n") ;
	setbuf(stderr, stream_buf) ;
	fprintf(stderr, "default stderr is nobuffer\n") ;
///	fflush(stderr) ; /* immedatle send buffer data to kernel, if stream is NULL clear all out stream */
	sleep(3) ;

	return 0 ;
}
#endif 


/*
 * freopen: function opens the file whose name is the string pointed
       to  by  path  and  associates the stream pointed to by stream with it.
       The original stream (if it exists) is closed.  The  mode  argument  is
       used  just  as  in  the fopen() function.  The primary use of the fre‐
       open() function is to change the file associated with a standard  text
       stream (stderr, stdin, or stdout).
	   关闭一个流，并重定向到一个文件
 */
#if 0
int main () {
	/* 把stdout重定向到outfile */
	FILE *os = freopen("outfile", "w", stdout) ;
	if (os == NULL) 
		perror("freopen error") ;

	fprintf(os, "hello\n") ;
	fprintf(stdout, "world\n") ; /* ok */

	/* closed os,  if stdout is vaivaile */
	S_TEST(0, fclose(os)) ;
	fprintf(stdout, "close os\n") ; /* run is ok, but not output*/
	TMP_MAX


	return 0 ;
}
#endif
/*
 * template file-name
 * ISO-C tmpnam, tmpfile
 * max counts invode: TMP_MAX
 * buf-length: at least L_tmpname
	* #ifdef _STDIO_H
# define L_tmpnam 20
# define TMP_MAX 238328
# define FILENAME_MAX 4096
 */
