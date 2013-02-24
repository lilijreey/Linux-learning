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
#include <sys/socket.h>
#include <resolv.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/x509.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#define MAXBUF 1024


char buf[MAXBUF];

unsigned long packMessage(char *message, const unsigned char command, const char *tokenBytes, const char *payload)
{
    unsigned long payloadLength = strlen(payload);
    unsigned short networkTokenLength = htons(32);
    unsigned short networkPayloadLength = htons(payloadLength);
 
    memcpy(message, &command, sizeof(unsigned char));
    message += sizeof(unsigned char);
    memcpy(message, &networkTokenLength, sizeof(unsigned short));
    message += sizeof(unsigned short);
    memcpy(message, tokenBytes, 32);
    message += 32;
    memcpy(message, &networkPayloadLength, sizeof(unsigned short));
    message += sizeof(unsigned short);
    memcpy(message, payload, payloadLength);
 
    return payloadLength + 37;
}


static int sendPayload(SSL *ssl, 
                       char *token, 
                       char *payloadBuff,
                       size_t payloadLength)
{
    int rtn = 1;
    char tokenBytes[32];
    char message[293];
    unsigned long msgLength;
 
    token2bytes(token, tokenBytes);
    msgLength = packMessage(message, 0, tokenBytes, payloadBuff);
    return SSL_write(ssl, message, (int)msgLength);
    return rtn;
}

void check_cert_chain(SSL * ssl)
{
    X509 *cert;
    char peer_CN[256];
    char sub[256];
    char iss[256];

    if (SSL_get_verify_result(ssl) != X509_V_OK) {
        printf("SSL_get_verify_result failed\n");
        exit(1);
    }

    cert = SSL_get_peer_certificate(ssl);
    if (cert != NULL) {
        printf("数字证书信息:\n");
        //答应server的DNS 域名 geteway.sandbox.push.app
        X509_NAME_get_text_by_NID(X509_get_subject_name(cert),
                                  NID_commonName, peer_CN, 256);

        X509_NAME_oneline(X509_get_subject_name(cert), sub, 256);
        X509_NAME_oneline(X509_get_issuer_name(cert), iss, 256);
        printf("peer_CN:%s\n sub:%s \n iss:%s\n", peer_CN, sub, iss);
        X509_free(cert);
    } else
        printf("没有数字证书\n");


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
    SSL_load_error_strings();
    ERR_load_BIO_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    OpenSSL_add_all_ciphers();
    ctx = SSL_CTX_new(SSLv23_client_method());
    if (ctx == NULL) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    //SSL_CTX_set_options(ctx, SSL_OP_ALL);


    //加载可信任证书库 AC
    if (0 == SSL_CTX_load_verify_locations(ctx, NULL, "/etc/ssl/certs")) {
        printf("err func:%s\n reaseon:%s", ERR_func_error_string(ERR_get_error()),
               ERR_reason_error_string(ERR_get_error()));
        ERR_print_errors_fp(stdout);
        exit(1);
    }

//    if (SSL_CTX_use_certificate_chain_file(ctx, "./ssl.pem") != 1) //Qus. 干嘛用的?
    /* 载入用户的数字证书， 此证书用来发送给客户端。 证书里包含有公钥 */
    if (SSL_CTX_use_certificate_file(ctx, "./ssl.pem", SSL_FILETYPE_PEM) != 1) {
        ERR_print_errors_fp(stdout);
        exit(1);
    }

    //设置私钥的使用密码
    SSL_CTX_set_default_passwd_cb_userdata(ctx, "Hx13917341682");
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


    //设置一个临时RSA，
//    RSA *rsa = RSA_generate_key(512, RSA_F4, NULL, NULL);
//    if (SSL_CTX_set_tmp_rsa(ctx, rsa) != 1) {
//        ERR_print_errors_fp(stdout);
//        exit(1);
//    }



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

//使用 BIO
//    BIO *bio = BIO_new_socket(sockfd, BIO_NOCLOSE);
//    if (NULL == bio) {
//        ERR_print_errors_fp(stderr);
//        goto finish;
//    }

    //把ssl和bio绑定
//    SSL_set_bio(ssl, bio, bio);
//

    //不使用BIO
    if (0 == SSL_set_fd(ssl, sockfd)) {  //使用了BIO 就不用这步了
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
    }

    //检测server身份
    check_cert_chain(ssl);

//    read_write(ssl, sockfd);

    char json[] = "{\"aps\":{\"alert\":\"Hello world!!!\",\"badge\":1}}";
//    char *json = "{\"aps\":{\"alert\":\"hello HHH\"}}";
 //   char *json = "{\"aps\":{\"badge\":123}}";

    char *token  ="aa994d28c89422d11c58ba4ab6d18309427699719e6f208395d0d5a40eb90d21";
    sendPayload(ssl, token, json, strlen(json));

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
