/*************************************************************************
	> File Name: test3.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月30日 星期二 20时21分01秒
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "apue.h"

static void sig_alrm(int signo)
{
	return ;
}

unsigned int sleep1(unsigned int nsecs)
{
	if(signal(SIGALRM,sig_alrm)==SIG_ERR)
	{
		return(nsecs);
	}
	
	alarm(nsecs);
	pause();
	
	return(alarm(0));
}


int main(int argc,char **argv)
{
	printf("111111\n");
	
	sleep1(2);
	
	printf("222222\n");

    return 0;
}

