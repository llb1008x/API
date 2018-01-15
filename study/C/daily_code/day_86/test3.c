/*************************************************************************
	> File Name: test3.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月15日 星期一 20时37分42秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "apue.h"

int main()
{
	pid_t pid;
	
	if( (pid=fork())<0 ){
		
		err_sys("fork error");
	// first chid
	}else if(pid==0){
		
		if( (pid=fork())<0 ){
		
			err_sys("fork error");
		}else if(pid>0){
			
			exit(0);
		}
		// second chid
		sleep(2);
		printf("second chid,paren pid=%d\n",getpid());
		exit(0);
	}
	
	if(waitpid(pid,NULL,0)!=pid){
		
		err_sys("waitpid error");
	}

    exit(0);
}

