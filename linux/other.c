/*
 * =====================================================================================
 *  set vim:foldmethod=marker
 *
 *       Filename:  c.c
 *
 *    Description:  
 *
 *        Created:  01/22/2013 10:22:44 AM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */


/// Assert 
//不想要的时候定义为
//(void) 1

/// bit segment
#if 0
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//Qus. 对一个bit段的溢出会影响其他bit段吗？
//  不会，而且还会出现警告, 最终的值是去除溢出bit的值
//{{{
struct 
{
    unsigned char a:2;
    unsigned char b:2;
} ok;

int main()
{
    ok.a=0;
    ok.b=0;

    printf("%d %d\n", ok.a, ok.b);
    ok.a=5;
    printf("%d %d\n", ok.a, ok.b);

    return 0;
}

#endif //}}}


//EE `getpwnam' , `getgrnam'
//  得到指定user的基本信息 {{{
#if 1
#include <pwd.h>
#include <string.h>
#include <stdio.h>
int main() {
    struct passwd * pw;
    char *username = "one";
    pw = getpwnam(username);
    if (!pw) {
        printf("%s is not exist\n", username);
        return -1;
    }
    printf("pw->pw_name   = %s\n", pw->pw_name);
    printf("pw->pw_passwd = %s\n", pw->pw_passwd);
    printf("pw->pw_uid    = %d\n", pw->pw_uid);
    printf("pw->pw_gid    = %d\n", pw->pw_gid);
    printf("pw->pw_gecos = %s\n", pw->pw_gecos);
    printf("pw->pw_dir    = %s\n", pw->pw_dir);
    printf("pw->pw_shell = %s\n", pw->pw_shell);

    return 0;
}

#endif 
// }}}

//EE `char* get_current_dir_name(void)';
//    得到PWD 调用者要free get_current_dir_name malloc 的buf
//    必须使用 _GNU_SOURCE 开能开启
#if 0
#define _GNU_SOURCE 
#include <unistd.h> //in _GNU_SOURCE {{{
int
main(int argc, char *argv[])
{
    char *pwd = get_current_dir_name();
    if (pwd == NULL) {
        perror("get_current_dir_name faild");
        exit(1);
    }
    printf("pwd:%s\n", pwd);

    free(pwd); /* !!! 调用者要负责free buf */
    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */

#endif // }}}

#if 0
///EE @c crypt/2 @c crypt_r
//linux 上密码加密算法 link with -lcrypt
//*EDS算法 默认方法。
//  被加密的key不能超过8个字节(如果超过8个实际情况和使用前8个最为key的值相同)
//  算法使用每个字符的第7个bit 创建一个56bit的密钥，用这个密钥加密,
//  最终的加密key长度为13B,最前面. 两个字符是调用crypt是传入的salt本身
//  
//原始的crypt使用EDS算法，glibc2 提供其他算法 
//  通过在第二个参数传递特殊string来选择其他算法,
//   第二个参数由两个部分组成ID, Salt,
//  Salt 的字符必须是  set [a–zA–Z0–9./]
//  $ID$Salt
//	  + ID  | 现在支持的其他算法 ID 
//	      1   | MD5
//	      2a  | Blowfish (not in mainline glibc; added in some Linux distributions)
//	      5   | SHA-256 (since glibc 2.7)
//	      6   | SHA-512 (since glibc 2.7)
//	  + Salt 是最长16B 的字符
//	加密后的string格式为$ID$Salt$Encrypted
//	 Encrypted 部分就是加密过后的key，长度是固定的
//       MD5     | 22 characters
//       SHA-256 | 43 characters
//       SHA-512 | 86 characters
//*MD5
//  $1$
// {{{

#define _XOPEN_SOURCE	   /* 需要_XOPEN_SOURCE */
#include <unistd.h>
#include <stdio.h>
#include <string.h>
int main()
{
    //使用默认的EDS算法 key的长度不能超过8B
    char key[] = "12345678";
    //第二个参数是从[a-zA-Z0-9]中选两个字符共有 4096种组合
    //返回一个加密后的key string, return NULL for error
    char *encryptKey = crypt(key, "PS");
    if (encryptKey == NULL)
        perror("crypt failed");
    printf("EDS encryptKey:%s length:%lu\n", encryptKey, strlen(encryptKey));


    //MD5
    encryptKey = crypt(key, "$1$yyy$");
    if (encryptKey == NULL)
        perror("crypt failed");
    printf("MD5 encryptKey:%s length:%lu\n", encryptKey, strlen(encryptKey));

    //SHA-256
    encryptKey = crypt(key, "$5$yyy$");
    if (encryptKey == NULL)
        perror("crypt failed");
    printf("SHA-256 encryptKey:%s length:%lu\n", encryptKey, strlen(encryptKey));
    return 0;
}
#endif //}}}
