/**
 * @file     rand.c
 *           伪随机数的实现原理和
 *           C stander lib 随即数的实现.
 *           
 *  数学原理：
 *           为了产生统计特征良好的为随机数，数学上一般采用递推公式
 *              X(n+1) = F(Xn, X(n-1), X(n-2), ..., X(n-k)))
 *           在给定了初始值X(0), X(-1), X(-2), ..., X(-k) 后可以
 *           逐步推出X1， X2, ...,Xn
 *           
 *  生产算法:
 *  + 平方取中法 
 *               将一个2s bit长度的数字N 平方后得到4s bit长度的数字S
 *               然后对这个S取中间的2s bit长度的数字作为下一个随即树
 *           递推公式： X(n+1) = (sqrt(X(n)) >> s) % 2^s
 *                         先平方然后右移s bit个长度，在莫 2的s次方
 *                   
 *           缺点：   分布不均匀，对初始值依赖大, 不能用于实际工作
 *           
 *  + 乘积取中法：
 *              对平方去中法的改进，需要两个初始值 X0， X1
 *              递推公式：  X(n+2) = ((X(n) * X(n+1)) >> s)  % 2^n
 *                        先把两个初始值相乘然后右移s bit个长度，
 *                        在莫 2的s次方
 *                      
 *           缺点：   对初始值依赖大不能用于实际工作
 *           
 *  + 移位法:
 *             如果是一个32位的数字， 将初始值 X(0) 左移7位得到X(L7)
 *             右移7为得到X(R7) 然后把X(L7) X(R7) 相加 莫 2^32 得到
 *             下一个随即数
 *             递推公式： X(n+1) = ((X(n) << 7） + (X(n) >> 7)) % 2^32
 *             取 Rn = X(n)* 2^-32 Rn 分布在[0,1] 区间上
 *           缺点：   对初始值依赖大, 一般要取大的初始值,不能用于实际工作
 *           
 *  + 同余法: 可以用于生产 LCG (linear congruential generator)
 *            线性递推公式：
 *            X(n+1) = (P*X(n) + C )(mode M), 其中R C M X(n) 都是正整数 
 *            产生的为随机数 Rn = X(n)/M 可以的出 0 <= X(n+1) < M
 *              R(n) 的区间在[0, 1)
 *     乘同余法：   C == 0             X(n+1) = P * X(n)     (mod M)
 *     加同余法：   C != 0 && P == 1   X(n+1) =     X(n) + C (mod M)
 *     混合同余法： C != 0 && P != 1  需要4个状态值也是初始值
 *                  大部分rand函数都用混合同余法来生产
 *          
 *          同余法对初始值和 C P M 的取值很重要。 一些选取C P M 的条件
 *          一个混合同余法中到达周期M的冲要条件：
 *                1. C 与 M 互素，
 *                2. 对于每一个M的素因子Q， P-1 为Q的倍数
 *                3. 若M是4的倍数，那P-1也是4的倍数
 *                对于在2进制的计算机上若M=2^s, P=4m + 1, C = 2n+1.
 *                  其中m,n为正整数
 *      总结：混合同余和乘法同余的效果较好
 *  
 *  + Mersenne twister算法: 高质量的随机数生成算法
 *                         Mersenne twister产生随机数的质量几乎超过任何LCG。
 *                         不过一般Mersenne twister的实现使用LCG产生种子。 
 *                         Mersenne twister是Makoto Matsumoto (松本)和Takuji Nishimura (西村)
 *                         于1997年开发的伪随机数产生器，
 *                         基于有限二进制字段上的矩阵线性再生。可以快速产生高质量的伪随机数
 *                         ，修正了古老随机数产生算法的很多缺陷。
 *                         Mersenne twister这个名字来自周期长度通常取Mersenne质数这样一个事实。
 *                         常见的有两个变种Mersenne Twister MT19937和Mersenne Twister MT19937-64。
 *                          Mersenne Twister有很多长处，
 *                          例如：周期2^19937 - 1对于一般的应用来说，
 *                          足够大了，序列关联比较小，能通过很多随机性测试。
 *                   
 *
 * @author   lili  <lilijreey@gmail.com>
 * @date     07/28/2013 09:13:18 AM
 *
 */


#define	_XOPEN_SOURCE			/*  */
#include <stdio.h>
#include <stdlib.h>

///stdlib 提供的其他随即函数 in POSIX.1-2001
///48 随即算法，都是使用LCG, 可以生成一系列48-bit 的整数
//
//48 系列函数使用的存放struct drand48_data
//  {
//    unsigned short int __x[3];	/* Current state.  */ 3* 16 = 48 
//    unsigned short int __old_x[3]; /* Old state.  */
//    unsigned short int __c;	/* Additive const. in congruential formula.  */
//    unsigned short int __init;	/* Flag for initializing. 标示data是否已经初始化 */
//    unsigned long long int __a;	/* Factor in congruential formula.  */
//  };
// 使用的公式  Xn+1 = (aXn + c) mod m, where n >= 0
//       m  = 2^48, hence 48-bit integer arithmetic is performed.
//	     a = 0x5DEECE66D
//	     c = 0xB
//	     
//主要实现代码
// 计算递推公式， buffer 代表一个随即4元组
// xsubi [IN|OUT] 传递当前的Xn值, 返回下一个值
int __drand48_iterate(unsigned short xsubi[3], 
                      struct drand48_data *buffer)
{
  uint64_t X;
  uint64_t result;
  /* Initialize buffer, if not yet done.  */
  if (__builtin_expect (!buffer->__init, 0))
    {
      buffer->__a = 0x5deece66dull;
      buffer->__c = 0xb;
      buffer->__init = 1;
    }

  /* Do the real work.  We choose a data type which contains at least
     48 bits.  Because we compute the modulus it does not care how
     many bits really are computed.  */

  X = (uint64_t) xsubi[2] << 32 | (uint32_t) xsubi[1] << 16 | xsubi[0];

  // A * Xn + c
  result = X * buffer->__a + buffer->__c;

  // do mod operate
  xsubi[0] = result & 0xffff;
  xsubi[1] = (result >> 16) & 0xffff;
  xsubi[2] = (result >> 32) & 0xffff;

  // 还剩下mod M
  return 0;
} 

//需要开启 _XOPEN_SOURCE, 都需要需要 srand48/1 设置种子
//+ drand48/0 -> double 输出[0.0, 1.0)之间的double 浮点数 
//            使用LCG 算法 48-bit 长度整形算法 
//
//+ lrand48/0 -> long int 返回的区间在[0, 2^31]
//+ mrand48/0 -> long int 返回的区间在[-2^31, 2^32]]
//....
 
///IOS C std lib rand 实现
//返回[0, RNAD_MAX]
int rand_r (unsigned int *seed)
{
  unsigned int next = *seed;
  int result;

  // 从中可以看出 也是用 LCG 算法 混合同余
  // a = 1103515245, c = 12345, M = 2^11
  // 对next计算三次递推，来分别得到result的0 ~ 10, 11 ~ 20, 21 ~ 30 
  // -> 0 ~ 30 = 31 bit
  next *= 1103515245;
  next += 12345;
  result = (unsigned int) (next / 65536) % 2048;

  next *= 1103515245;
  next += 12345;
  result <<= 10;
  result ^= (unsigned int) (next / 65536) % 1024;

  next *= 1103515245;
  next += 12345;
  result <<= 10;
  result ^= (unsigned int) (next / 65536) % 1024;

  *seed = next;

  return result;
}

int main()
{
    for (int i=0; i <=10; ++i)
        printf("drand48 return:%lf\n", drand48());

    return 0;
}
