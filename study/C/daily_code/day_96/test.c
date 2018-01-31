/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月31日 星期三 18时46分36秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <setjmp.h>
#include <errno.h>
#include "apue.h"

static jmp_buf env_alrm;

static void sig_alrm(int signo)
{
	longjmp(env_alrm,1);
}


unsigned int sleep1(unsigned nsecs)
{
	if(signal(SIGALRM,sig_alrm)==SIG_ERR){
	
		return(nsecs);
	}
	
	alarm(nsecs);
	pause();
	
	return(alarm(0));
}

unsigned int sleep2(unsigned int nsecs)
{
	if(signal(SIGALRM,sig_alrm)==SIG_ERR){
	
		return(nsecs);
	}
	
	if(setjmp(env_alrm)==0){
		
		alarm(nsecs);
		pause();
	}
	
	return(alarm(0));
}

static void sig_int(int signo)
{
	int i;
	volatile int j;
	
	printf("\n sig_int starting \n");
	for(i=0;i<200000;i++){
		j+= i*i;
	}
	
	printf("sig_int finishned\n");

	return ;
}


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


int main(int argc,char **argv)
{
	unsigned int unslept;
	
	if(signal(SIGINT,sig_int)==SIG_ERR){
		err_sys("signal sigint error");
	}
	
	pr_mask("SIGALRM");
	
	unslept=sleep2(5);
	printf("sleep2 return %u \n",unslept);
	
    return 0;
}

