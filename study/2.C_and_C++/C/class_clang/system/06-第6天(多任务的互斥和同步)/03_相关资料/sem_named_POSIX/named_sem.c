#include<stdio.h>
#include<semaphore.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>

void printer(sem_t *sem, char *str)
{
	sem_wait(sem);	//信号量减一
	while(*str!='\0')
	{
		putchar(*str);	
		fflush(stdout);
		str++;
		sleep(1);
	}
	printf("\n"); 
	sem_post(sem);	//信号量加一
}


int main(int argc, char *argv[])
{
	pid_t pid;
	sem_t *sem = NULL;
	
	pid = fork();
	if(pid<0)
	{
		perror("fork error");
	}
	else if(pid==0)
	{
		sem = sem_open("name_sem", O_CREAT|O_RDWR, 0666, 1);//跟open打开方式很相似,不同进程只要名字一样，那么打开的就是同一个有名信号量
		if(sem == SEM_FAILED)
		{
			perror("sem_open");
			return -1;
		}
		char *str1 = "hello";
		printer(sem, str1);
		sem_close(sem);
		_exit(1);
	}
	else
	{
		sem = sem_open("name_sem", O_CREAT|O_RDWR, 0666, 1);//跟open打开方式很相似,不同进程只要名字一样，那么打开的就是同一个有名信号量
		if(sem == SEM_FAILED)
		{
			perror("sem_open");
			return -1;
		}
		char *str2 = "world";
		printer(sem, str2);
		sem_close(sem);//关闭有名信号量
		
		wait(pid,NULL);
	}
	
	sem_unlink("name_sem");//删除有名信号量
	return 0;
}