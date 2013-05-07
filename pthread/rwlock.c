/**
 * @file     rwlock.c
 *
 * @brief    读写锁 pthread_rwlock_*
 *
 * @author   lili  lilijreey@gmail.com
 * @date     05/06/2013 01:52:19 PM
 *
 */
#include  <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include	<sys/types.h>
#include	<pthread.h>
#include	<stdlib.h>
#include	<zhao/tools.h>

#define COUNT 100000

pthread_rwlock_t rwl;
int lock_sum;

/// 2个线程同时写一个共享数据，使用RWL保护
void* lock_add(void *arg)
{
    for (int i=0; i< COUNT; ++i) {
		PCHECK(pthread_rwlock_wrlock(&rwl));
        ++lock_sum;
		PCHECK(pthread_rwlock_unlock(&rwl));
    }
    printf("thread:%lu lock_sum:%d\n", (unsigned long)pthread_self(), lock_sum);
    return 0;
}

int main(void)
{
    PCHECK(pthread_rwlock_init(&rwl, NULL));
	pthread_t ptid[2];
    PCHECK(pthread_create(&ptid[0], NULL, lock_add, NULL));
    PCHECK(pthread_create(&ptid[1], NULL, lock_add, NULL));


    for (int i=0; i<2; ++i)
        pthread_join(ptid[i], NULL/*not care thread return code */);

    printf("lock_sum:%d\n", lock_sum);

    return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
