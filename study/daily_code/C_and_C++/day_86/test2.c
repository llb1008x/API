/*************************************************************************
	> File Name: test2.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月15日 星期一 20时17分31秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "apue.h"


void pr_exit(int status)
{
	if(WIFEXITED(status)){
		
		printf("normal termination ,exit status=%d\n",WEXITSTATUS(status));
	}else if(WIFSIGNALED(status)){
	
		printf("abnormal termination ,signal number=%d%s\n",
			WTERMSIG(status),
		#if WCOREDUMP
			WCOREDUMP(status)?"(core file generated)": " ");
		#else
			"");
		#endif	
	}else if(WIFSTOPPED(status)){
		
		printf("child stopped,signal number=%d\n",WSTOPSIG(status));
	}	
}

int main()
{
	pid_t pid;
	int status;

// 1	
	if( (pid=fork())<0 ){
		
		err_sys("fork error");
	/*chid*/
	}else if(pid==0){
	
		exit(7);
	}
	
	if(wait(&status)!=pid){
	
		err_sys("wait error");
	}
	pr_exit(status);
	
// 2
	if( (pid=fork())<0 ){
		
		err_sys("fork error");
	/*chid*/
	}else if(pid==0){
	
		abort();
	}
	
	if(wait(&status)!=pid){
	
		err_sys("wait error");
	}
	pr_exit(status);
	
// 3
	if( (pid=fork())<0 ){
		
		err_sys("fork error");
	/*chid*/
	}else if(pid==0){
	
		status /= 0;
	}
	
	if(wait(&status)!=pid){
	
		err_sys("wait error");
	}
	pr_exit(status);		

    exit(0);
}
