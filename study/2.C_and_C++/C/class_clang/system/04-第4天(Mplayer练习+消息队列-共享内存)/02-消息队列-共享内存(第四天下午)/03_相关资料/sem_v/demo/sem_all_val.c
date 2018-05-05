
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
	key_t key;

	key = ftok(".", 'a');
	if(key == -1)
	{
		perror("ftok");
	}
	system("ipcs -s");
	int semid;

	semid = semget(key, 2, IPC_CREAT|0666);
	if(semid == -1)
	{
		perror("semget");
	}
	system("ipcs -s");
	int ret;

	unsigned short sem_arry[2] = {30,20};
	ret = semctl(semid, 0, SETALL, sem_arry);
	if(ret == -1)
	{
		perror("semctl");
	}
	bzero(sem_arry, sizeof(sem_arry));
	ret = semctl(semid, 0, GETALL, sem_arry);
	if(ret == -1)
	{
		perror("semctl");
	}
	printf("sem_arry[0] = %d\n", sem_arry[0]);
	printf("sem_arry[1] = %d\n", sem_arry[1]);

	semctl(semid, 1, IPC_RMID);
	system("ipcs -s");
	
	return 0;
}

