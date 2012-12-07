/*
 * =====================================================================================
 *
 *       Filename:  sockopt.c
 *
 *    Description: Get default socket opt
 *
 *        Created:  12/07/2012 11:52:14 AM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */

   #include <sys/types.h>	       /* See NOTES */
   #include <sys/socket.h>

#include <stdio.h>
#include	<stdlib.h>

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int
main(int argc, char *argv[])
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)  {
        perror("socket:");
        return 1;
    }

    socklen_t l= sizeof(int);
   int v=0;
   if ( -1 == getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &v, &l)) {
       perror("getsockopt:");
       return -1;
   }
   printf("SO_RCVBUF: %d\n", v);

   if ( -1 == getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &v, &l)) {
       perror("getsockopt:");
       return -1;
   }
   printf("SO_SNDBUF: %d\n", v) ;


#define	GET_OPT(opt) \
   if ( -1 == getsockopt(sockfd, SOL_SOCKET, opt, &v, &l)) \
   { perror("getsockopt:"); return -1; } \
   printf(": %d\n", v) 

   GET_OPT(SO_RCVTIMEO);
   GET_OPT(SO_SNDTIMEO);
   return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
