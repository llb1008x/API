#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int my_system(const char * cmdstring)
{
	pid_t pid;
	int status = 0;
	if(cmdstring == NULL)
	{
		return (1);
	}
	pid = fork();
	if(pid < 0)
	{
		status = -1;
	}
	else if(pid == 0)
	{
		execl("/bin/sh", "sh", "-c", cmdstring, NULL);
		perror("execl error");
		_exit(127); //子进程正常执行则不会执行此语句
	}
	else
	{
		while(waitpid(pid, &status, 0) < 0)
		{
			if(errno != EINTR)
			{
				status = -1;
				break;
			}
		}
	}
	return status;
}


int main(int argc, char *argv[])
{
	int status = 0;
	
	while(1)
	{
		char cmd[100] = "";
		printf("cmd:");
		fflush(stdout);
		fgets(cmd, sizeof(cmd), stdin);
		if(cmd[strlen(cmd)-1] == '\n')
		{
			cmd[strlen(cmd)-1] = 0;
		}
		status = my_system(cmd);
		//printf("status=======%x\n", status);
	}
	return 0;
}
