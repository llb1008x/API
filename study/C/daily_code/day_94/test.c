/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月29日 星期一 14时28分27秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include "apue.h"

static void sig_hup(int );
static void pr_ids(char *);


int main()
{
	//printf("hello world!\n");
    char c;
    pid_t pid;
    
    pr_ids("parent");
    
    if((pid=fork())<0){
    	err_sys("fork error");
    }else if(pid>0){
    	
    	printf("parent pid->%d\n",getpid());
    	sleep(5);
    	exit(0);
    }else {
    	
    	printf("child pid->%d\n",getpid());
    	
    	pr_ids("child");
    	signal(SIGHUP,sig_hup);
    	kill(getpid(),SIGTSTP);
    	pr_ids("child");
    	
    	if(read(0,&c,1)!=1){
    		
    		printf("read error from control terminal,errno=%d\n",errno);
    	}
    	
    	exit(0);
    }
   
    
    return 0;
}


static void sig_hup(int signo)
{
	printf("SIGHUP received ,pid=%d\n",getpid());
	return ;
}

static void pr_ids(char *name)
{
	printf("%s:pid=%d,ppid=%d,pgrp=%d\n",name,getpid(),getppid(),getpgrp());
	
	fflush(stdout);
}
