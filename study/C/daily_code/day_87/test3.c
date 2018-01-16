/*************************************************************************
	> File Name: test3.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月16日 星期二 17时06分53秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int main()
{
	pid_t pid;
	
	if((pid=fork())<0){
		
		err_sys("fork error");
	}else if(pid==0){
	
		if(execl("/home/llb/project/echoall",
				"test","myargl","MT ARG2",(char *)0)<0);
			err_sys("execl error");	
	}
	
	if(waitpid(pid,NULL,0)<0){
		
		err_sys("waitpid error");
	}
	
    return 0;
}

