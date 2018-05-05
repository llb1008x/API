/*************************************************************************
	> File Name: test2.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月30日 星期二 19时37分04秒
 ************************************************************************/

#include <stdio.h>
#include <pwd.h>
#include <signal.h>
#include "apue.h"

static void my_alarm(int );

int main(int argc,char **argv)
{
	struct passwd *ptr;
	signal(SIGALRM,my_alarm);
	alarm(1);
	
	while(1){
	
		if((ptr=getpwnam("steven"))==NULL){
			err_sys("getpwnam error");
		}
		
		if(strcmp(ptr->pw_name,"steven")!=0){
		
			printf("return value corrupt!pw_name=%s\n",ptr->pw_name);
		}
	}

    return 0;
}

static void my_alarm(int signo)
{
	struct passwd *rootptr;
	
	printf("in signal handler\n");
	if((rootptr=getpwnam("root"))==NULL){
		err_sys("getpwnam(root) error");
	}
	
	alarm(1);
	
	return ;
}
