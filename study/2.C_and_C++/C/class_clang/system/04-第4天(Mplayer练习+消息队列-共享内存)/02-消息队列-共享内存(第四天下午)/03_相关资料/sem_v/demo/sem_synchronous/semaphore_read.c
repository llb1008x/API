#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void sem_v(int semid, int flag)
{  
	int ret;
	struct sembuf tmp;

	tmp.sem_num = flag;
	tmp.sem_op = -1;
	tmp.sem_flg = 0;
	
	//int semop(int semid, struct sembuf *sops, unsigned nsops);
	//nsops为sops 指向的struct sembuf结构体的个数
	ret = semop(semid, &tmp, 1);
	if(ret == -1)
	{
		perror("wait semop");
	}
}

void sem_p(int semid, int flag)
{ 
	int ret;
	struct sembuf tmp;

	tmp.sem_num = flag;
	tmp.sem_op = 1;
	tmp.sem_flg = 0;

	ret = semop(semid, &tmp, 1);
	if(ret == -1)
 	{
		perror("post semop");
	}
}

void sem_i(int semid)
{
	semctl(semid, 0, SETVAL, 0);
	semctl(semid, 1, SETVAL, 1);
}

int main(int argc, char *argv[])
{
	int semid;
	key_t key;
	int i = 0;
	
	key = ftok(".", 'a');
	if(key==-1)
	{
		perror("ftok");
	}
	semid = semget(key, 2, 0777|IPC_CREAT);	
	if(semid < 0) 
	{ 
		perror("semget"); 
	}
	sem_i(semid);
	while(1)
	{
		sem_v(semid, 0);
		printf("in read i = %d\n", i++);
		sem_p(semid, 1);
	}
	return 0;
}

