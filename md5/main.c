/**
 * @file     main.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     07/27/2013 03:40:02 PM
 *
 */

#include <stdio.h>
#include <string.h>
#include "md5calc.h"

int main()
{
    char input[] = "1234567";
    char md5V[80];

    MD5_String(input, md5V);
    printf("md5:%s\n", md5V);
    printf("md5-length:%lu\n", strlen(md5V));
    return 0;

}
