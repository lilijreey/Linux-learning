/**
 * @file     writer.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     02/13/2014 11:45:04 AM
 *
 */
#include <stdlib.h>
#include <rpc/xdr.h>


int
main(int argc, char *argv[])
{
    XDR xdr;

    //把编码后的数据输出到stdout, 由于编码后是不可见字符，所以使用
    // ./w | hexdump 输出
    xdrstdio_create(&xdr, stdout, XDR_ENCODE);
    for (long i =0; i < 8; ++i)
    {
        if ( !xdr_long(&xdr, &i) ) {
            fprintf(stderr, "failed");
            exit(1);
        }

    }

    return 0;
}				/* ----------  end of function main  ---------- */
