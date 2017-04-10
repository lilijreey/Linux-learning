/*
 * =====================================================================================
 *
 *       Filename:  sem1.c
 *
 *    Description:  use to inter-process semaphore
 *
 *        Version:  1.0
 *        Created:  04/10/2011 10:14:51 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lilijreey (), lilijreey@Gmail.com
 *        Company:  
 *
 * =====================================================================================
 */

#include	<unistd.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<sys/sem.h>

#include	"myUtility.h"
  
//define union semun
union semun {
	int val ;
	struct semid_ds *buf;
	unsigned short *array ;
	struct seminfo *__buf ;
} ;


static void del_semvalue(int sem_id)
{
	union semun sem_union ; 

	sem_union.val = 1;

	if(-1 == semctl(sem_id, 0, IPC_RMID, 1))
		fprintf(stderr, "Failed to delete samephore\n") ;
}

static int semaphore_p(int sem_id) 
{
	struct sembuf sem_ops ;
	
	sem_ops.sem_num = 0 ;
	sem_ops.sem_op = -1 ;
	sem_ops.sem_flg = SEM_UNDO ;

	if(-1 == semop(sem_id, &sem_ops, 1)) {
		return -1 ;
	} else {
		return 0 ;
	}
}

static int semaphore_v(int sem_id) 
{
	struct sembuf sem_ops ;
	
	sem_ops.sem_num = 0 ;
	sem_ops.sem_op = 1 ;
	sem_ops.sem_flg = SEM_UNDO ;

	if(-1 == semop(sem_id, &sem_ops, 1)) {
		return -1 ;
	} else {
		return 0 ;
	}
}

int main(int argc, char *argv[])
{
	int sem_id ;
	char op_char = 'O' ;
	int i ;
	int pause_time ;

	srand((unsigned int)getpid()) ;

	//create a semaphore 
	EV_TEST(-1, sem_id, semget((key_t)123, 1, 0666 | IPC_CREAT))
	
	if(argc > 1) {
		//init semaphore
		E_TEST(-1, semctl(sem_id, 0, SETVAL, 1))
		op_char = 'X' ;
		sleep(4) ;
	}

	for(i = 0; i < 10; i++) {
		//enter critical section
		S_TEST(0, semaphore_p(sem_id))

		printf("%c", op_char) ;
		fflush(stdout) ;
		sleep(rand() % 3) ;
		printf("%c", op_char) ;
		fflush(stdout) ;

		//leaves critical section
		S_TEST(0, semaphore_v(sem_id)) 

		pause_time = rand() % 2 ;
		sleep(pause_time) ;
	}

	printf("\n%d - finished\n", getpid()) ;

	if(argc > 1) {
		sleep(10) ;
		del_semvalue(sem_id) ;
	}

	return 0 ;
}
