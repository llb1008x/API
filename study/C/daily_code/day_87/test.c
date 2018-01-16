/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月16日 星期二 11时37分04秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
//#include "ourhdr.h"

static void charatatime(char *str)
{
	char *ptr;
	int c;
	
	setbuf(stdout,NULL);
	for(ptr=str;c=*ptr++; ){
		
		putc(c,stdout);
	}
}

int main()
{
	pid_t pid;
	TELL_WAIT();

	if((pid=fork())<0){
		err_sys("fork error");
/*chid*/	
	}else if(pid==0){
		//printf("1 pid->%d,ppid->%d\n",getpid(),getppid());
		
		//printf("2 pid->%d,ppid->%d\n",getpid(),getppid());
		//sleep(1);
		//WAIT_PARENT();
		charatatime("output from child\n");
		TELL_PARENT(getppid());
		exit(0);
	}
	
/*parent*/
	//sleep(2);	
	
	WAIT_CHILD();
	//sleep(1);
	charatatime("output from father\n");
	//TELL_CHILD(pid);

    exit(0);
}

