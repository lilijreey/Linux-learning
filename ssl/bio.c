/*
 * =====================================================================================
 *
 *       Filename:  bio.c
 *
 *    Description:  
 *
 *        Created:  01/22/2013 09:24:07 PM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#define MAXBUF 1024

static char *token  ="aa994d28c89422d11c58ba4ab6d18309427699719e6f208395d0d5a40eb90d21";


char buf[MAXBUF];
static int sendPayload(BIO *bio,
                       char *token, 
                       char *payloadBuff,
                       size_t payloadLength)
{
    int rtn = 1;
    if (bio && token && payloadBuff && payloadLength)
    {
        uint16_t token_size = strlen(token);
        int data_size = 1 + 2 + token_size + 2 + payloadLength;
        printf("send data size:%u\n", data_size);

        char *binaryMessagePt = buf;
        uint16_t networkOrderTokenLength = htons(token_size);
        uint16_t networkOrderPayloadLength = htons(payloadLength);

        /* command */
        uint8_t command = 0; /* command number */
        *binaryMessagePt++ = command;

        /* token length network order */
        memcpy(binaryMessagePt, &networkOrderTokenLength, sizeof(uint16_t));
        binaryMessagePt += sizeof(uint16_t);
        /* device token */
        memcpy(binaryMessagePt, token, token_size);
        binaryMessagePt += token_size;

        /* payload length network order */
        memcpy(binaryMessagePt, &networkOrderPayloadLength, sizeof(uint16_t));
        binaryMessagePt += sizeof(uint16_t);

        /* payload */
        memcpy(binaryMessagePt, payloadBuff, payloadLength);
        binaryMessagePt += payloadLength;

        /* 发消息给服务器 */
        rtn = BIO_write(bio, buf, data_size);
        if (rtn < 0) {
            printf ("消息发送失败！");
        }
        else
            printf("消息发送成功, 共发送了%d个字节！\n", rtn);
    }
    return rtn;
}

// EE非安全链接
#if 0
int main() 
{
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();

    BIO *bio = BIO_new_connect("gateway.sandbox.push.apple.com:2195");
    if (bio == NULL) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    if (BIO_do_connect(bio) <= 0) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    char buf[MAXBUF];
    char *json = "{\"aps\":{\"badge\":123}}";
    sendPayload(bio, token, json, strlen(json));
    int ret = BIO_read(bio, buf, MAXBUF);
    if (ret <= 0) {
        printf("BIO_read return 0\n");
    }

    BIO_free_all(bio);
    return 0;
}
#endif

// 安全链接， 1.验证服务器 2验证客户端

int main() 
{
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    OpenSSL_add_all_algorithms();
    
    SSL_CTX *ctx = SSL_CTX_new(SSLv23_client_method());
    if (ctx == NULL) {
        printf("SSL_CTX_new err func:%s\n reaseon:%s", ERR_func_error_string(ERR_get_error()),
               ERR_reason_error_string(ERR_get_error()));
        exit(1);
    }

    //加载可信任证书库
    if (0 == SSL_CTX_load_verify_locations(ctx, "./push_cer.pem", NULL)) {
        printf("err func:%s\n reaseon:%s", ERR_func_error_string(ERR_get_error()),
               ERR_reason_error_string(ERR_get_error()));
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    //set BIO
    BIO *bio = BIO_new_ssl_connect(ctx);
    if (bio == NULL) {
        printf("err func:%s\n", ERR_func_error_string(ERR_get_error()));
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    SSL *ssl;
    BIO_get_ssl(bio, &ssl);
    SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

    //open safe connect
    BIO_set_conn_hostname(bio, "gateway.sandbox.push.apple.com:2195");

    //verify connect ok
    if (BIO_do_connect(bio) <= 0) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    if (SSL_get_verify_result(ssl) != X509_V_OK) {
        printf("SSL_get_verify_result not success\n");
    }

    char buf[MAXBUF];
    char *json = "{\"aps\":{\"badge\":123}}";
    sendPayload(bio, token, json, strlen(json));
    int ret = BIO_read(bio, buf, MAXBUF);
    if (ret <= 0) {
        printf("BIO_read return 0\n");
    }

    SSL_CTX_free(ctx);
    BIO_free_all(bio);
    return 0;
}

