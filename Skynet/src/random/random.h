/**
 *============================================================
 *  @file      random.h
 *  @brief     用于产生随机数。
 * 
 *  compiler   gcc4.1.2
 *  platform   Linux
 *
 *============================================================
 */

#ifndef LIBant_RANDOM_HPP_
#define LIBant_RANDOM_HPP_

#include <stdlib.h>

/**
 * @brief 产生给定范围内的随机数。如果需要更加随机，可以在程序起来的时候调用一次srand()。
 * @param min 随机出来的最小数。
 * @param max 随机出来的最大数。
 * @return min和max之间的随机数，包括min和max。
 */
static inline int ranged_random(int min, int max)
{
    // generates ranged random number
    return (rand() % (max - min + 1)) + min;
}

#endif // LIBant_RANDOM_HPP_
