
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <stdio.h>

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

	semid = semget(key, 1, IPC_CREAT|0666);
	if(semid == -1)
	{
		perror("semget");
	}
	system("ipcs -s");
	
	int ret;

	ret = semctl(semid, 0, SETVAL, 0);
	if(ret == -1)
	{
		perror("semctl");
	}

	//P操作
	struct sembuf sops;
	sops.sem_num = 0;
	sops.sem_op = -1;
	sops.sem_flg = 0;
	ret = semop(semid, &sops, 1);
	if(ret == -1)
	{
		perror("semctl");
	}
	return 0;
}
