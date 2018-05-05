/*************************************************************************
	> File Name: system_test.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月13日 星期三 19时03分58秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <error.h>
#include <signal.h>

#include "apue.h"

static void sig_int(int signoo)
{
	
	printf("interrupt\n%%");
}


int main()
{
	
	printf("uid->%d,pid->%d\n",getuid(),getpid());

#if 1
	char buf[10];
	pid_t pid;
	int status;
	
	//SIG_ERR ctrl+c
	if(signal(SIGINT,sig_int)==SIG_ERR){
		err_sys("signal error");
	}
	
	printf("pid -> %d \n",getpid());
	printf("%% ");
	while(fgets(buf,10,stdin)!=NULL){
	
		buf[strlen(buf)-1]=0;
		printf("buf->%s\n",buf);
		
		if((pid=fork()) < 0){
			printf("pid1 -> %d \n",getpid());
			err_sys("fork error ");
		}else if(pid == 0){
			
			printf("pid2 -> %d \n",getpid());
			execlp(buf,buf,(char *)0);
			err_ret("could't execute:%s",buf);
			exit(127);
		}
		printf("pid3 -> %d \n",getpid());
		/*parent*/
		if( (pid=waitpid(pid,&status,0))<0){
			printf("pid4 -> %d \n",getpid());
			err_sys("waitpid error");
		}
		
		printf("%%");
	}
#endif 
    exit(0);
}
