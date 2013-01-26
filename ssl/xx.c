/*
 * =====================================================================================
 *
 *       Filename:  xx.c
 *
 *    Description:  
 *
 *        Created:  01/24/2013 10:55:56 AM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */

/*
 * =====================================================================================
 *
 *       Filename:  ssl.c
 *
 *    Description:  
 *
 *        Created:  01/22/2013 01:38:40 PM
 *         Author:  YOUR NAME (), 
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <time.h>
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <openssl/crypto.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#define MAXBUF 1024

static char *token  ="aa994d28c89422d11c58ba4ab6d18309427699719e6f208395d0d5a40eb90d21";


char buf[MAXBUF];
static int sendPayload(SSL *sslPtr, 
                       char *token, 
                       char *payloadBuff,
                       size_t payloadLength)
{
    int rtn = 1;
    if (sslPtr && token && payloadBuff && payloadLength)
    {
        uint16_t token_size = strlen(token);
        int data_size = 1 + 2 + token_size + 2 + payloadLength;
        printf("send data size:%d token_size%u psize:%lu\n", 
               data_size, token_size, payloadLength);

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
        rtn = SSL_write(sslPtr, buf, data_size);
        if (rtn < 0) {
            printf ("消息发送失败！");
        }
        else
            printf("消息发送成功, 共发送了%d个字节！\n", rtn);
    }
    return rtn;
}

static int sendPayloadEnhanced(SSL *sslPtr, 
                       char *token, 
                       char *payloadBuff,
                       size_t payloadLength)
{
    int rtn = 1;
    if (sslPtr && token && payloadBuff && payloadLength)
    {
        uint16_t token_size = strlen(token);
        int data_size = 1 + 4 + 4 + 2 + token_size + 2 + payloadLength;
        printf("send data size:%d token_size%u psize:%lu\n", 
               data_size, token_size, payloadLength);

        char *binaryMessagePt = buf;
        uint16_t networkOrderTokenLength = htons(token_size);
        uint16_t networkOrderPayloadLength = htons(payloadLength);

        /* command */
        uint8_t command = 1; /* command number */
        *binaryMessagePt++ = command;

        uint32_t id=123;
        memcpy(binaryMessagePt, &id, sizeof(uint32_t));
        binaryMessagePt += sizeof(uint32_t);

        uint32_t expire = htonl(time(NULL) + 3200);
        memcpy(binaryMessagePt, &expire, sizeof(uint32_t));
        binaryMessagePt += sizeof(uint32_t);

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

        assert (binaryMessagePt - buf == data_size && "not ==");
        /* 发消息给服务器 */
        rtn = SSL_write(sslPtr, buf, data_size);
        if (rtn < 0) {
            printf ("消息发送失败！");
        }
        else
            printf("消息发送成功, 共发送了%d个字节！\n", rtn);
    }
    return rtn;
}
void ShowCerts(SSL * ssl)
{
    X509 *cert;
    char *line;

    cert = SSL_get_peer_certificate(ssl);
    if (cert != NULL) {
        printf("数字证书信息:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("证书: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("颁发者: %s\n", line);
        free(line);
        X509_free(cert);
    } else
        printf("无证书信息！\n");
}

int main(int argc, char **argv)
{
    int sockfd, len;
    struct sockaddr_in dest;
    char buffer[MAXBUF + 1];
    SSL_CTX *ctx;

    if (argc != 3) {
        printf
            ("参数格式错误！正确用法如下：\n\t\t%s IP地址 端口\n\t比如:\t%s 127.0.0.1 80\n此程序用来从某个 IP 地址的服务器某个端口接收最多 MAXBUF 个字节的消息",
             argv[0], argv[0]);
        exit(0);
    }

    /* SSL 库初始化，参看 ssl-server.c 代码 */
 //   SSLeay_add_all_algorithms();
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    ctx = SSL_CTX_new(SSLv23_client_method());
    if (ctx == NULL) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }


    //加载可信任证书库 AC
    if (0 == SSL_CTX_load_verify_locations(ctx, NULL, "/etc/ssl/certs")) {
        printf("err func:%s\n reaseon:%s", ERR_func_error_string(ERR_get_error()),
               ERR_reason_error_string(ERR_get_error()));
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    /* 载入用户的数字证书， 此证书用来发送给客户端。 证书里包含有公钥 */
    if (SSL_CTX_use_certificate_file(ctx, "./ssl.pem", SSL_FILETYPE_PEM) != 1) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    /* 载入用户私钥 */
    if (SSL_CTX_use_PrivateKey_file(ctx, "./pri.pem", SSL_FILETYPE_PEM) != 1) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    /* 检查用户私钥是否正确 */
    if (!SSL_CTX_check_private_key(ctx)) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }


//    struct host *hptr = gethostbyname("gateway.sandbox.push.apple.com");
//    if (hptr == NULL) {
//        perror("gethostbyname faild");
//        exit(1);
//    }
//
//    printf("addrs:%s\n",hptr->h_name);


    /* 创建一个 socket 用于 tcp 通信 */
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket");
        exit(errno);
    }

    /* 初始化服务器端（对方）的地址和端口信息 */
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(atoi(argv[2]));
    if (inet_aton(argv[1], (struct in_addr *) &dest.sin_addr.s_addr) == 0) {
        perror(argv[1]);
        exit(errno);
    }

    /* 连接服务器 */
    if (connect(sockfd, (struct sockaddr *) &dest, sizeof(dest)) != 0) {
        perror("Connect failde");
        exit(errno);
    }
    printf("server connected success\n");

    /* 基于 ctx 产生一个新的 SSL */
    SSL *ssl;
    if (NULL == (ssl = SSL_new(ctx))) {
        ERR_print_errors_fp(stderr);
        goto finish;
    }

    if (0 == SSL_set_fd(ssl, sockfd)) {
        printf("SSL_set_fd error\n");
        ERR_print_errors_fp(stderr);
        exit(1);
    }

    /* 建立 SSL 连接 */
    if (SSL_connect(ssl) != 1) {
        printf("SSL_connect faild\n");
        ERR_print_errors_fp(stderr);
        goto finish;
    }
    else {
        printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
        ShowCerts(ssl);
    }

    char *json = "{\"aps\" : { \"alert\" : \"hello world\"}}";
 //   char *json = "{\"aps\":{\"badge\":123}}";

   // sendPayload(ssl, token, json, strlen(json));
    sendPayloadEnhanced(ssl, token, json, strlen(json));

    /* 接收对方发过来的消息，最多接收 MAXBUF 个字节 */
    bzero(buffer, MAXBUF + 1);
    /* 接收服务器来的消息 */
    len = SSL_read(ssl, buffer, MAXBUF);
    if (len > 0)
        printf("接收消息成功:'%s'，共%d个字节的数据\n", buffer, len);
    else if (len == 0) 
        printf("close peer\n");
    else {
        ERR_print_errors_fp(stderr);
        int ret = SSL_get_error(ssl, len);
        if (ret == SSL_ERROR_WANT_WRITE) {
            printf("read a no data nonblock fd\n");
        }
        printf ("消息接收失败！错误代码是%d，错误信息是'%s'\n", errno, strerror(errno));

        goto finish;
    }


finish:
    /* 关闭连接 */
//    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sockfd);
    SSL_CTX_free(ctx);
    return 0;
}
