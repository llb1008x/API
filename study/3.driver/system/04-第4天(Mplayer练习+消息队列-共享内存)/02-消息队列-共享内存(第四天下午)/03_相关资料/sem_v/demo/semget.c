
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

	semid = semget(key, 2, IPC_CREAT|0666);
	if(semid == -1)
	{
		perror("semget");
	}
	system("ipcs -s");
	semctl(semid, 1, IPC_RMID);
	system("ipcs -s");
	
	return 0;
}

