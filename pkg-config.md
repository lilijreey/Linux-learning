
使用 
pkg-config 有默认的所搜路径

pkg-config --list-all
  显示所有pkg-config 可以找到的pkg

pkg-config --exists  <pkgName> 检测一个包是否能找到， @? 返回
or 
pkg-config --exists --print-erors <pkgName> 如果没有输出就说明找到了

.pc 文件所搜路径变量
PKG_CONFIG_PAT

pkg-config 


pkgconfig 
实现原理

要求所有lib提供一个 .pc 文件
一般放在 /usr/lib/pkgconfig
pc 文件搜索路径
 PKG_CONFIG_PATH


e.g. libcurl

prefix=/usr/local
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${prefix}/include
supported_protocols="DICT FILE FTP FTPS GOPHER HTTP HTTPS IMAP IMAPS LDAP LDAPS POP3 POP3S RTMP RTSP SMB SMBS SMTP SMTPS TELNET TFTP"
supported_features="SSL IPv6 UnixSockets libz IDN NTLM NTLM_WB TLS-SRP"

Name: libcurl
URL: http://curl.haxx.se/
Description: Library to transfer files with ftp, http, etc.
Version: 7.42.1
Libs: -L${libdir} -lcurl
Libs.private: -lidn -lrtmp -lssl -lcrypto -lssl -lcrypto -llber -lldap -lz
Cflags: -I${includedir} 
