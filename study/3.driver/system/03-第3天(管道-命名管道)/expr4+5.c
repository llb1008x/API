#include<stdio.h>
#include<unistd.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	int ret = 0;
	int fd[2] = {0};
	
	ret = pipe(fd);
	if(ret == -1)
	{
		perror("pipe");
		_exit(1);
	}
	
	pid = vfork();
	if(pid < 0)
	{
		perror("vfork");
	}
	else if(pid == 0)
	{
		char *arg[] = {"expr", "4", "+", "5", NULL};
		dup2(fd[1], 1);//��׼����ض��򵽹ܵ���д��
		execvp(arg[0], arg);
		//execlp("expr", "expr", "4", "+", "5", NULL);
		
		perror("execvp");
		_exit(-1);
	}
	else
	{
		char result[100] = "";
		read(fd[0], result, sizeof(result));//�ӹܵ��Ķ��˶�ȡ����
		printf("result===%s\n", result);
	}

	return 0;
}