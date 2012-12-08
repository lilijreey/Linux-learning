/**
 *============================================================
 *  @file      atomic.h
 *  @brief     从内核代码里抽取出来的整型原子操作函数/宏。
 *
 * 
 *  platform   Linux
 *
 *============================================================
 */

#ifndef LIBant_ARCH_I386_ATOMIC_
#define LIBant_ARCH_I386_ATOMIC_

#define LOCK "lock ; "

/**
 * @brief This struct makes sure gcc doesn't try to be clever and
 *        move things around on us. We need to use _exactly_ the address
 *        the user gave us, not some alias that contains the same information.
 */
typedef struct
{
  volatile int counter;
} atomic_t;

/**
 * @brief Read atomic variable. Atomically reads the value of v.
 * @param v pointer of type atomic_t
 */
#define atomic_read(v)		((v)->counter)

/**
 * @brief Set atomic variable. Atomically sets the value of v to i.
 * @param v pointer of type atomic_t
 * @param i required value
 */
#define atomic_set(v,i)		(((v)->counter) = (i))

/**
 * @brief Add integer to atomic variable. Atomically adds i to v.
 * @param i integer value to add
 * @param v pointer of type atomic_t
 */
static __inline__ void
atomic_add (int i, atomic_t * v)
{
  __asm__ __volatile__ (LOCK "addl %1,%0":"=m" (v->counter):"ir" (i),
			"m" (v->counter));
}

/**
 * @brief Subtract the atomic variable. Atomically subtracts i from v.
 * @param i integer value to subtract
 * @param v pointer of type atomic_t
 */
static __inline__ void
atomic_sub (int i, atomic_t * v)
{
  __asm__ __volatile__ (LOCK "subl %1,%0":"=m" (v->counter):"ir" (i),
			"m" (v->counter));
}

/**
 * @brief Subtract value from variable and test result\n
 *        Atomically subtracts i from v and returns
 *        true if the result is zero, or false for all
 *        other cases.
 * @param i integer value to subtract
 * @param v pointer of type atomic_t
 */
static __inline__ int
atomic_sub_and_test (int i, atomic_t * v)
{
  unsigned char c;

  __asm__ __volatile__ (LOCK "subl %2,%0; sete %1":"=m" (v->counter),
			"=qm" (c):"ir" (i), "m" (v->counter):"memory");
  return c;
}

/**
 * @brief Increment atomic variable. Atomically increments v by 1.
 * @param v pointer of type atomic_t
 */
static __inline__ void
atomic_inc (atomic_t * v)
{
  __asm__ __volatile__ (LOCK "incl %0":"=m" (v->counter):"m" (v->counter));
}

/**
 * @brief Decrement atomic variable. Atomically decrements v by 1.
 * @param v pointer of type atomic_t
 */
static __inline__ void
atomic_dec (atomic_t * v)
{
  __asm__ __volatile__ (LOCK "decl %0":"=m" (v->counter):"m" (v->counter));
}

/**
 * @brief Decrement and test.\n
 *        Atomically decrements v by 1 and
 *        returns true if the result is 0, or false for all other
 *        cases.
 * @param v pointer of type atomic_t
 */
static __inline__ int
atomic_dec_and_test (atomic_t * v)
{
  unsigned char c;

  __asm__ __volatile__ (LOCK "decl %0; sete %1":"=m" (v->counter),
			"=qm" (c):"m" (v->counter):"memory");
  return c != 0;
}

/**
 * @brief Increment and test.\n
 *        Atomically increments v by 1
 *        and returns true if the result is zero, or false for all
 *        other cases.
 * @param v pointer of type atomic_t
 */
static __inline__ int
atomic_inc_and_test (atomic_t * v)
{
  unsigned char c;

  __asm__ __volatile__ (LOCK "incl %0; sete %1":"=m" (v->counter),
			"=qm" (c):"m" (v->counter):"memory");
  return c != 0;
}

/**
 * @brief Add and test if negative.\n
 *        Atomically adds i to v and returns true
 *        if the result is negative, or false when
 *        result is greater than or equal to zero.
 * @param v pointer of type atomic_t
 * @param i integer value to add
 */
static __inline__ int
atomic_add_negative (int i, atomic_t * v)
{
  unsigned char c;

  __asm__ __volatile__ (LOCK "addl %2,%0; sets %1":"=m" (v->counter),
			"=qm" (c):"ir" (i), "m" (v->counter):"memory");
  return c;
}

/**
 * @brief Add and return. Atomically adds i to v and returns i + v
 * @param i integer value to add
 * @param v pointer of type atomic_t
 */
static __inline__ int
atomic_add_return (int i, atomic_t * v)
{
  int __i;
#ifdef CONFIG_M386
  if (unlikely (boot_cpu_data.x86 == 3))
    goto no_xadd;
#endif
  /* Modern 486+ processor */
  __i = i;
  __asm__ __volatile__ (LOCK "xaddl %0, %1;":"=r" (i):"m" (v->counter),
			"0" (i));
  return i + __i;

#ifdef CONFIG_M386
no_xadd:			/* Legacy 386 processor */
  local_irq_disable ();
  __i = atomic_read (v);
  atomic_set (v, i + __i);
  local_irq_enable ();
  return i + __i;
#endif
}

/**
 * @brief Subtract and return. Atomically subtracts i from v and returns i - v
 * @param i integer value to add
 * @param v pointer of type atomic_t
 */
static __inline__ int
atomic_sub_return (int i, atomic_t * v)
{
  return atomic_add_return (-i, v);
}

/**
 * @brief Increment and return
 * @param v pointer of type atomic_t
 */
#define atomic_inc_return(v)  (atomic_add_return(1,v))
/**
 * @brief Decrement and return
 * @param v pointer of type atomic_t
 */
#define atomic_dec_return(v)  (atomic_sub_return(1,v))

/* These are x86-specific, used by some header files */
#define atomic_clear_mask(mask, addr) \
__asm__ __volatile__(LOCK "andl %0,%1" \
: : "r" (~(mask)),"m" (*addr) : "memory")

#define atomic_set_mask(mask, addr) \
__asm__ __volatile__(LOCK "orl %0,%1" \
: : "r" (mask),"m" (*(addr)) : "memory")

#endif // LIBant_ARCH_I386_ATOMIC_
