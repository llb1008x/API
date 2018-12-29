/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月17日 星期三 09时57分38秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

pid_t pid,pid1,pid2;

int main(int argc,char *argv[])
{

	int fd[2];
	char outpipe[100],inpipe[100];
	
	if(pipe(fd)<0){
	
		perror("pipe error!");
		return -1;
	}
	
	if( (pid=fork())<0){
	
		perror("fork pid error");
		return -1;
	}else if(pid==0){
	
		printf("child pid is %d\n",getpid());
		close(fd[0]);
		strcpy(outpipe,"child is sending a message");
		
		if(write(fd[1],outpipe,50)== -1){
		
			perror("child 1 write to outpipe error");
			return -1;
		}
		
		exit(0);
	}
	
	if((pid2=fork())<0){
	
		perror("fork pid2 error ");
		return -1;
	}else if(pid2==0){
	
		printf("child2 pid is %d\n",getpid());
		close(fd[0]);
		strcpy(outpipe,"child2 is sending a message!");
		sleep(1);
		
		if(write(fd[1],outpipe,50)== -1){
			perror("child2 write to outpipe error");
			return -1;
		}
		exit(0);
	}
	
	close(fd[1]);
	pid=wait(NULL);
	printf("%d process is over!\n",pid);
	if(read(fd[0],inpipe,50)== -1){
		perror("read child1 pipe error");
		return -1;
	}
	
	printf("%s\n",inpipe);
	
	pid=wait(NULL);
	printf("%d process is over!\n",pid);
	
	if(read(fd[0],inpipe,50)==-1){
		perror("read child1 pipe error");
		return -1;
	}
	
	printf("%s\n",inpipe);
	
    exit(0);
}
