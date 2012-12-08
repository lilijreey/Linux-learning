/**
 *============================================================
 *  @file      hash_algo.h
 *  @brief     哈希算法
 * 
 *  compiler   gcc4.1.2
 *  platform   Linux
 *
 *============================================================
 */

#include <stdint.h>

/**
 * @brief 根据输入的字符串，生成一个32位的哈希值
 * @param p 需要生成哈希值的字符串
 * @return 字符串对应的哈希值
 */
static inline uint32_t r5hash(const char* p)
{
	uint32_t h = 0;
	while (*p) {
		h = h * 11 + (*p << 4) + (*p >> 4);
		p++;
	}
	return h;
}

