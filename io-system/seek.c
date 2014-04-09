/*
 * =====================================================================================
 *
 *       Filename:  seek.c
 *
 *    Description:  about seek option and effect
 *
 *        Created:  08/28/2012 10:43:42 PM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */

//#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
       #include <sys/types.h>
       #include <sys/stat.h>

//EE get file size two function
/* second effect change file offset */
static int file_size1(int fd) 
{
    //fd must offset = 0
    off_t file_size = lseek(fd, 0, SEEK_END);
    return file_size;
}

/*
 * no second effect
 */
static int file_size2(int fd)
{
    struct stat st;
    fstat(fd, &st);

    return st.st_size;
}



// 使用无缓冲I/O
//Qus. 打开一个有内容的file，把文件偏移量移动的开始，写入的数据会覆盖原来的
//     数据吗？ 
//     会把原来的内容覆盖

// EE 打开文件是除非是指定 O_APPEN, 文件偏移量都为0

// EE linux 提供两个函数可以用来代替lseek + read/ lseek+write
//   `pread' `pwrite'
//   这两个函数提供Positional Reads and Writes
//   并且不改变file的position,
//   还能避免position races.
int main()
{
    int ft = open("./orgfile", O_RDWR);
//    E_TEST(-1, ft);
//    off_t offset = lseek(ft, 0, SEEK_SET); // set offset to begin file opsition 
//    E_TEST((off_t) -1, offset);
//    printf("offset is:%ld\n", offset);
//
//    char buf[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; // '\0' is last charater
//    write(ft, buf, sizeof(buf));
//    E_TEST(-1, close(ft));

//    fpos_t pos;
//    FILE *f = fopen("./dup1.c", "r");
//    fgetpos(f, &pos);
//    printf("pos :%lu\n", pos);

    int size1 = file_size1(ft);
    int size2 = file_size2(ft);
    printf("s1:%d s2:%d\n", size1, size2);

    return 0;
}

