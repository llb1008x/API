/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月30日 星期二 16时09分13秒
 ************************************************************************/

#include <stdio.h>
#include <signal.h>
#include "apue.h"

static void sig_usr(int ); 

int main(int argc,char **argv)
{
	if(signal(SIGUSR1,sig_usr)==SIG_ERR){
		
		err_sys("can't catch sigusr!");
	}
	
	if(signal(SIGUSR2,sig_usr)==SIG_ERR){
	
		err_sys("can't catch sigusr!");
	}
	
	while(1){
		
		pause();
	}


    return 0;
}


static void sig_usr(int signo)
{
	if(signo==SIGUSR1)
		printf("received SIGUSR1");
		
	else if(signo==SIGUSR2)
		printf("received SIGUSR2");
		
	else 
		err_dump("received signal %d\n",signo);
		
	return ;			
}
