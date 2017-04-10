/**
 * @file     hash.c
 *           
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     02/19/2014 04:24:26 PM
 *
 * hash int , pointer string func
 *
 */
#include <stdlib.h>
#include <stdio.h>


// from kernel 
// 
/* 2^31 + 2^29 - 2^25 + 2^22 - 2^19 - 2^16 + 1 */
#define golden_ratio_prime_32 0x9e370001ul
/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#define golden_ratio_prime_64 0x9e37fffffffc0001ul

#if __WORDSIZE ==64 
#   define hash_long(val, bits) hash_64(val, bits)
#   define golden_ratio_prime golden_ratio_prime_64
#else /* 32 */
#   define golden_ratio_prime golden_ratio_prime_32
#   define hash_long(val, bits) hash_32(val, bits)
#endif


/* @param bits use to get the high bits the hash value
 * like mod e.g. int buf[1<<3]; hash_32(323242, 3)
 *  */
static inline unsigned long hash_64(unsigned long val, unsigned int bits)
{
	unsigned long hash = val;

	/*  sigh, gcc can't optimise this alone like it does for 32 bits. */
	unsigned long n = hash;
	n <<= 18;
	hash -= n;
	n <<= 33;
	hash -= n;
	n <<= 3;
	hash += n;
	n <<= 3;
	hash -= n;
	n <<= 4;
	hash += n;
	n <<= 2;
	hash += n;

	/* high bits are more random, so use them. */
	return hash >> (64 - bits);
}

static inline unsigned long hash_32(unsigned long val, unsigned int bits)
{
	/* on some cpus multiply is faster, on others gcc will do shifts */
	unsigned long hash = val * golden_ratio_prime_32;

	/* high bits are more random, so use them. */
	return hash >> (32 - bits);
}

static inline unsigned long hash_ptr(const void *ptr, unsigned int bits)
{
	return hash_long((unsigned long)ptr, bits);
}

//static inline unsigned long hash32_ptr(const void *ptr)
//{
//	unsigned long val = (unsigned long)ptr;
//
//#if __WORDSIZE == 64
//	val ^= (val >> 32);
//#endif
//	return (u32)val;
//}


/// 
unsigned int hash_str(const void *str)
{
	unsigned int h = 5381;
	const char *p = (const char *)str;

	while (*p != '\0')
		h = ((h << 5) + h) + (*p++); /* hash * 33 + c */

	return h;
}


int
main(int argc, char *argv[])
{
    printf("3232:%ld\n", hash_long(32423, 5));
    printf("3232:%ld\n", hash_long(32423, 3));
    printf("3232:%ld\n", hash_long(32423, 32));
    printf("3hello:%u\n", hash_str("3hello"));
    printf("3hel3o:%u\n", hash_str("3hillo"));

    return 0;
}				/* ----------  end of function main  ---------- */
