/*************************************************************************
	> File Name: test3.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月31日 星期三 20时33分16秒
 ************************************************************************/

#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <time.h>
#include "apue.h"

static void 					sig_usr1(int ),sig_alrm(int );
static sigjmp_buf 				jmpbuf;
static volatile	sig_atomic_t 	canjump;


int main(int argc,char **argv)
{
	if(signal(SIGUSR1,sig_usr1)==SIG_ERR){
		err_sys("signal SIGUSR1 error");
	}
	
	if(signal(SIGALRM,sig_alrm)==SIG_ERR){
		err_sys("signal SIGALRM error");
	}
	
	pr_mask("starting main:");
	if(sigsetjmp(jmpbuf,1)){
		
		pr_mask("ending main:");
		exit(0);
	}
	
	canjump=1;
	while(1){
		
		pause();
	}
	
    return 0;
}

static void sig_usr1(int signo)
{
	time_t starttime;
	
	if(canjump==0){
		
		return ;
	}
	
	pr_mask("staring sig_usr1:");
	alarm(3);
	
	starttime=time(NULL);
	while(1){
		if(time(NULL)>starttime+5){
			
			break;
		}
	}
	
	pr_mask("finishing sig_usr1:");
	canjump=0;
	
	siglongjmp(jmpbuf,1);
}

static void sig_alrm(int signo)
{
	pr_mask("in sig_alrm:");
	return ;
}
