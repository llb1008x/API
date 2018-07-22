#include<stdio.h>
#include<semaphore.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>

void printer(sem_t *sem, char *str)
{
	sem_wait(sem);	//�ź�����һ
	while(*str!='\0')
	{
		putchar(*str);	
		fflush(stdout);
		str++;
		sleep(1);
	}
	printf("\n"); 
	sem_post(sem);	//�ź�����һ
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
		sem = sem_open("name_sem", O_CREAT|O_RDWR, 0666, 1);//��open�򿪷�ʽ������,��ͬ����ֻҪ����һ������ô�򿪵ľ���ͬһ�������ź���
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
		sem = sem_open("name_sem", O_CREAT|O_RDWR, 0666, 1);//��open�򿪷�ʽ������,��ͬ����ֻҪ����һ������ô�򿪵ľ���ͬһ�������ź���
		if(sem == SEM_FAILED)
		{
			perror("sem_open");
			return -1;
		}
		char *str2 = "world";
		printer(sem, str2);
		sem_close(sem);//�ر������ź���
		
		wait(pid,NULL);
	}
	
	sem_unlink("name_sem");//ɾ�������ź���
	return 0;
}