/**
 * @file     test.c
 *           
 */

#define _XOPEN_SOURCE	   /* See feature_test_macros(7) */
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <zhao/tools.h>

///  创建(打开)伪终端设备有多种接口
void createPty() {
    int masterFd;
    //same as posix_openpt 返回主设备句柄
    EV_TEST(-1, masterFd, open("/dev/ptmx", O_RDWR | O_NOCTTY));
    printf("savle dives name:%s\n", ptsname(masterFd));
    //得到从设备名称后,在打开
    /*close(masterFd);*/
}

//systemV
// posix_openpt
//BSD
// openpty
// tty_login
// forkpty = opentpy + fork + tty_login


int main(int argc, char *argv[])
{
    createPty();
    
    return 0;
}

