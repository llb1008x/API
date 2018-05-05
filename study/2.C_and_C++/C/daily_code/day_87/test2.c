/*************************************************************************
	> File Name: test2.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月16日 星期二 15时08分09秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

char *env_init[]={"user=unknown","PATH=/tmp",NULL};

int main(int argc,char *argv[])
{
	pid_t pid;
	
	if( (pid=fork())<0 ){
	
		err_sys("fork error");
/*child*/	
	}else if(pid==0){
		
		printf("at here 1 pid1->%d\n",getpid());
		if(execle("/home/llb/project/API/study/C/daily_code/day_87/tmp/echoall",
			"echoall","myargl","MY ARG2",(char *)0,
			 env_init)<0)
		  err_sys("execle error");	     
	}
	
	printf("at here 2 pid2->%d\n",getpid());
	
	if(waitpid(pid,NULL,0)<0){
		
		err_sys("wait error");
	}
	
	if( (pid=fork())<0){
		
		err_sys("fork error");
	}else if(pid==0){
	
		printf("at here 3 pid3->%d\n",getpid());
		if(execlp("tmp/echoall","echoall","only 1 arg",(char *)0)<0);
		
		err_sys("execlp error");
	}
	
	printf("at here 4 pid4->%d\n",getpid());
	
    exit(0);
}


