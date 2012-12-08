/**
 *============================================================
 *  @file      ipaddr_cat.h
 *  @brief     ip地址分类
 * 
 *  platform   Linux
 *============================================================
 */

#ifndef IPADDR_CAT_H_
#define IPADDR_CAT_H_

#include <arpa/inet.h>
#include <netinet/in.h>

/**
 * @brief 判断给定的ip地址是否内网地址
 * @param ip 网络序的ip地址
 * @return 0不是内网地址，1内网地址
 */
static inline int is_private_ip2(in_addr_t ip)
{
	return (((ip & 0x0000FFFF) == 0xA8C0)
			|| ((ip & 0x000000FF) == 0x0A)
			|| ((ip & 0x0000F0FF) == 0x10AC));
}

/**
 * @brief 判断给定的ip地址是否内网地址
 * @param ip ip地址
 * @return 0不是内网地址，1内网地址，-1给定地址不合法
 */
static inline int is_private_ip(const char* ip)
{
	in_addr_t ipaddr;
	if (inet_pton(AF_INET, ip, &ipaddr) == 1) {
		return is_private_ip2(ipaddr);
	}

	return -1;
}

/**
 * @brief 判断给定的ip地址是否loopback地址
 * @param ip 网络序的ip地址
 * @return 0不是loopback，1是loopback
 */
static inline int is_loopback_ip2(in_addr_t ip)
{
	return ((ip & 0x000000FF) == 0x7F);
}

/**
 * @brief 判断给定的ip地址是否loopback地址
 * @param ip ip地址
 * @return 0不是，1是，-1给定地址不合法
 */
static inline int is_loopback_ip(const char* ip)
{
	in_addr_t ipaddr;
	if (inet_pton(AF_INET, ip, &ipaddr) == 1) {
		return is_loopback_ip2(ipaddr);
	}

	return -1;
}

/**
 * @brief 判断给定的ip地址是否zero地址
 * @param ip 网络序的ip地址
 * @return 0不是，1是
 */
static inline int is_zero_ip2(in_addr_t ip)
{
	return ((ip & 0x000000FF) == 0x00);
}

/**
 * @brief 判断给定的ip地址是否zero地址
 * @param ip ip地址
 * @return 0不是，1是，-1给定地址不合法
 */
static inline int is_zero_ip(const char* ip)
{
	in_addr_t ipaddr;
	if (inet_pton(AF_INET, ip, &ipaddr) == 1) {
		return is_zero_ip2(ipaddr);
	}

	return -1;
}

/**
 * @brief 判断给定的ip地址是否zeroconf地址
 * @param ip 网络序的ip地址
 * @return 0不是，1是
 */
static inline int is_zeroconf_ip2(in_addr_t ip)
{
	return ((ip & 0x0000FFFF) == 0xFEA9);
}

/**
 * @brief 判断给定的ip地址是否zeroconf地址
 * @param ip ip地址
 * @return 0不是，1是，-1给定地址不合法
 */
static inline int is_zeroconf_ip(const char* ip)
{
	in_addr_t ipaddr;
	if (inet_pton(AF_INET, ip, &ipaddr) == 1) {
		return is_zeroconf_ip2(ipaddr);
	}

	return -1;
}

/**
 * @brief 判断给定的ip地址是否multicast地址
 * @param ip 网络序的ip地址
 * @return 0不是，1是
 */
static inline int is_multicast_ip2(in_addr_t ip)
{
	return ((ip & 0x000000F0) == 0xE0);
}

/**
 * @brief 判断给定的ip地址是否multicast地址
 * @param ip ip地址
 * @return 0不是，1是，-1给定地址不合法
 */
static inline int is_multicast_ip(const char* ip)
{
	in_addr_t ipaddr;
	if (inet_pton(AF_INET, ip, &ipaddr) == 1) {
		return is_multicast_ip2(ipaddr);
	}

	return -1;
}

/**
 * @brief 判断给定的ip地址是否保留地址
 * @param ip 网络序的ip地址
 * @return 0不是，1是
 */
static inline int is_reserved_ip2(in_addr_t ip)
{
	return ((ip & 0x000000F0) == 0xF0);
}

/**
 * @brief 判断给定的ip地址是否保留地址
 * @param ip ip地址
 * @return 0不是，1是，-1给定地址不合法
 */
static inline int is_reserved_ip(const char* ip)
{
	in_addr_t ipaddr;
	if (inet_pton(AF_INET, ip, &ipaddr) == 1) {
		return is_reserved_ip2(ipaddr);
	}

	return -1;
}

/**
 * @brief 判断给定的ip地址是否ipv6到ipv4的relay anycast地址
 * @param ip 网络序的ip地址
 * @return 0不是，1是
 */
static inline int is_relay_ip2(in_addr_t ip)
{
	return ((ip & 0x00FFFFFF) == 0x6358C0);
}

/**
 * @brief 判断给定的ip地址是否ipv6到ipv4的relay anycast地址
 * @param ip ip地址
 * @return 0不是，1是，-1给定地址不合法
 */
static inline int is_relay_ip(const char* ip)
{
	in_addr_t ipaddr;
	if (inet_pton(AF_INET, ip, &ipaddr) == 1) {
		return is_relay_ip2(ipaddr);
	}

	return -1;
}


/**
 * @brief 判断给定的ip地址是否网络设备benchmark地址
 * @param ip 网络序的ip地址
 * @return 0不是，1是
 */
static inline int is_benchmark_ip2(in_addr_t ip)
{
	return ((ip & 0x0000FEFF) == 0x12C6);
}

/**
 * @brief 判断给定的ip地址是否网络设备benchmark地址
 * @param ip ip地址
 * @return 0不是，1是，-1给定地址不合法
 */
static inline int is_benchmark_ip(const char* ip)
{
	in_addr_t ipaddr;
	if (inet_pton(AF_INET, ip, &ipaddr) == 1) {
		return is_benchmark_ip2(ipaddr);
	}

	return -1;
}

/**
 * @brief 判断给定的ip地址是否documentation and examples地址
 * @param ip 网络序的ip地址
 * @return 0不是，1是
 */
static inline int is_doc_ip2(in_addr_t ip)
{
	return ((ip & 0x00FFFFFF) == 0x0200C0);
}

/**
 * @brief 判断给定的ip地址是否documentation and examples地址
 * @param ip ip地址
 * @return 0不是，1是，-1给定地址不合法
 */
static inline int is_doc_ip(const char* ip)
{
	in_addr_t ipaddr;
	if (inet_pton(AF_INET, ip, &ipaddr) == 1) {
		return is_doc_ip2(ipaddr);
	}

	return -1;
}

#endif // IPADDR_CAT_H_

