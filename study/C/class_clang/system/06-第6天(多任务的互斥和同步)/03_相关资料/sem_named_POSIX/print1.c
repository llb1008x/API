
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <stdio.h>

void print(sem_t *print1, sem_t *print2)
{
	int i = 0;
	while(1)
	{
		sem_wait(print1);
		i++;
		printf("int print1 i = %d\n", i);
		sem_post(print2);
	}
}

int main(int argc, char **argv)
{	
	sem_t *print1, *print2;

	print1 = sem_open("sem_print1", O_CREAT, 0777, 0);  
	if(SEM_FAILED == print1)
	{
		perror("sem_open");
	}
	print2 = sem_open("sem_print2", O_CREAT, 0777, 1);    
	if(SEM_FAILED == print2)
	{
		perror("sem_open");
	}
	print(print1, print2);
	return 0;
}

