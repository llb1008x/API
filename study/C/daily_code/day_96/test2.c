/*************************************************************************
	> File Name: test2.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月31日 星期三 19时35分22秒
 ************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include "apue.h"

void pr_mask(const char *str)
{
	sigset_t sigset;
	int errno_save;
	
	errno_save=errno;
	
	if(sigprocmask(0,NULL,&sigset)<0){
	
		err_sys("sigpromask error");
	}
	
	printf("%s",str);
	
	if(sigismember(&sigset,SIGINT))	printf("SIGINT");
	if(sigismember(&sigset,SIGINT))	printf("SIGOUT");
	if(sigismember(&sigset,SIGINT))	printf("SIGUSR1");
	if(sigismember(&sigset,SIGINT))	printf("SIGALRM");
	
	printf("\n");
	errno=errno_save;
}

static void sig_quit(int signo)
{
	printf("caught SIGQUIT\n");
	
	if(signal(SIGQUIT,SIG_DFL)==SIG_ERR){
		
		err_sys("can't reset SIGQUIT");
	}
	
	return ;
}

int main(int argc,char **argv)
{
	sigset_t newmask,oldmask,pendmask;
	
	if(signal(SIGQUIT,sig_quit)==SIG_ERR){
	
		err_sys("can't catch SIGQUIT");
	}
	
	sigemptyset(&newmask);
	sigaddset(&newmask,SIGQUIT);
	if(sigprocmask(SIG_BLOCK,&newmask,&oldmask)<0){
		err_sys("SIG_BLOCK error");
	}
	
	sleep(5);
	
	if(sigpending(&pendmask)<0){
		err_sys("sigpendmask error");
	}
	
	if(sigismember(&pendmask,SIGQUIT)){
		
		printf("\n SIGQUIT pending\n");
	}
	
	if(sigprocmask(SIG_SETMASK,&oldmask,NULL)<0){
	
		err_sys("SIG_SERMASK error");
	}
	
	printf("SIGQUIT unblocked\n");
	
	sleep(5);
	
    return 0;
}

