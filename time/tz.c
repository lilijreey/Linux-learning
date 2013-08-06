/**
 * @file     tz.c
 *           时区的get and set
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     05/10/2013 02:45:12 PM
 *
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    time_t t = time(NULL);
    struct tm tt;
    localtime_r(&t, &tt);
    printf("tm_isdst：%d\n", tt.tm_isdst);
    printf("tzname:%s\n", tzname[0]);
//    printf("timezone:%d\n", daylight);
//    printf("TZ:%s\n", getenv("TZ"));
    return 0;
}

