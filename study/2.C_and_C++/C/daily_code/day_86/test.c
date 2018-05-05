/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月15日 星期一 19时03分48秒
 
 f o r k有两种用法:
(1) 一个父进程希望复制自己,使父、子进程同时执行不同的代码段。这在网络服务进程
中是常见的——父进程等待委托者的服务请求。当这种请求到达时,父进程调用 f o r k,使子进
程处理此请求。父进程则继续等待下一个服务请求。
(2) 一个进程要执行一个不同的程序。这对 s h e l l是常见的情况。在这种情况下,子进程在
从f o r k返回后立即调用e x e c (我们将在8 . 9节说明e x e c )。
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include "apue.h"

int glob=6;
char buf[]="a write to stdout";

int main(int argc,char *argcgv[])
{
	int var;
	pid_t pid;
	
	var=88;
	
	//printf("before fork 1\n");
	
	if( write(STDOUT_FILENO,buf,sizeof(buf)-1) != (sizeof(buf)-1) ){
	
		err_sys("write error");
	}
	
	printf("before fork2\n");
	if( (pid=vfork()) < 0 ){
	
		err_sys("fork error");
	}else if(pid==0){
	
		glob++;
		var++;
		exit(0);
	}else{
	
		sleep(1);
	}
	
	printf("pid->%d,glob->%d,var->%d\n",getpid(),glob,var);
	
  	exit(0);
}
