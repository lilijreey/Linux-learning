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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//EE `getpwnam' , `getgrnam'
//  得到指定user的基本信息 {{{
#if 0
#include <pwd.h>
#include <string.h>
#include <stdio.h>
int main() {
    struct passwd * pw;
    char *username = "zhaoli";
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
#define _GNU_SOURCE 
#include <unistd.h> //in _GNU_SOURCE {{{
#if 1
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

