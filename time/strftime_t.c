/**
 * @file     strftime_t.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     02/21/2014 01:31:19 PM
 *
 */
#include <stdlib.h>

#include <stdio.h>
#include <time.h>
//       size_t strftime(char *s, size_t max, const char *format,
//		       const struct tm *tm);

       int main(int argc, char *argv[])
       {
	   char outstr[100];
	   time_t t;
	   struct tm tmp;

//       int i;
	   t = time(NULL);
       localtime_r(&t, &tmp);


       //%Y year mounth %m day %d
       if (strftime(outstr, sizeof(outstr), "%M", &tmp) == 0) {
//	   if ((i=strftime(outstr, sizeof(outstr),	 "%a, %d %b %Y %T %z", &tmp)) == 0) {
	       fprintf(stderr, "strftime returned 0");
	       exit(EXIT_FAILURE);
	   }

	   printf("Result string n: is \"%s\"\n",  outstr);
   exit(EXIT_SUCCESS);
       }
