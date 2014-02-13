/**
 * @file     reader.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     02/13/2014 11:47:35 AM
 *
 */
#include <stdlib.h>

#include <rpc/xdr.h>

//read decode from ./w and encode
int
main(int argc, char *argv[])
{
    XDR xdr;
    long j;

    xdrstdio_create(&xdr, stdin, XDR_DECODE);
    for (int i =0; i < 8; ++i)
    {
        if ( !xdr_long(&xdr, &j) ) {
            fprintf(stderr, "failed");
            exit(1);
        }
        printf("%ld\n", j);
    }
    printf("\n");
    return 0;
}

