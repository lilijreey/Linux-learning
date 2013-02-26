/*
 * =====================================================================================
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



//EE getpwnam , getgrnam
//  得到指定user的基本信息
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
