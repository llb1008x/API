
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
/* 解决编译出错的问题*/
#define IPC_INFO 3

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
	struct seminfo buf;

	semctl(semid, 0, IPC_INFO, &buf);
	printf("buf.semmni = %d\n", buf.semmni);
	printf("buf.semmns = %d\n", buf.semmns);
	printf("buf.semmnu = %d\n", buf.semmnu);
	printf("buf.semmsl = %d\n", buf.semmsl);
	printf("buf.semopm = %d\n", buf.semopm);
	printf("buf.semume = %d\n", buf.semume);
	printf("buf.semusz = %d\n", buf.semusz);
	printf("buf.semvmx = %d\n", buf.semvmx);
	printf("buf.semaem = %d\n", buf.semaem);

	semctl(semid, 1, IPC_RMID);
	system("ipcs -s");
	
	return 0;
}

