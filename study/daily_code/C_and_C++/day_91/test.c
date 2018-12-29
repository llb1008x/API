/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月26日 星期五 16时28分37秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"


int main(int argc,char **argv)
{
	pid_t pid;
	
	if((pid=fork())<0){
	
		err_sys("fork error");
	}else if(pid==0){
	
		if(execl("/home/llb/project/API/study/C/daily_code/day_91/testinterp","testinterp","myargl","MY ARG2",NULL)<0)
			
			err_sys("execl error");
	}
	
	if(waitpid(pid,NULL,0)<0){
	
		err_sys("waitpid error");
	}


    exit(0);
}

