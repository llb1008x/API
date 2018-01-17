/*************************************************************************
	> File Name: create.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月17日 星期三 15时30分12秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

char *env_init[]={"user=unknown","PATH=/tmp",NULL};

int main(int argc,char *argv[])
{
	pid_t pid1,pid2;

	if(argc<2){
		
		printf("usage %s <filename>\n",argv[0]);
		return -1;
	}
	
	if(mkfifo(argv[1],0664)<0){
	
		perror("mkfifo fails");
		exit(-1);
	}
	
#if 0	
	if((pid1=fork())<0){
	
		printf("fork error\n");
		exit(-1);
/*child 1*/	
	}else if(pid1==0){
		
		printf("child 1 arrive here !!!\n");
		if(execle("/home/llb/project/API/study/C/daily_code/day_88/write",
			"work","hello",(char *)0,env_init<0))
			
				err_sys("execle error 1");	
		
		return -1;
	}
	
	
	if((pid2=fork())<0){
	
		printf("fork error\n");
		return -2;
/*child 2*/	
	}else if(pid2==0){
		if(execle("/home/llb/project/API/study/C/daily_code/day_88/read",
			"work","world",(char *)0,env_init<0))
		
			err_sys("execlp error 2");
		
		
		exit(-2);	
	}
	
	
	if(waitpid(pid1,NULL,0)<0){
		
		err_sys("wait error ,child did not game over");
	}
#endif 	

    return 0;
}
