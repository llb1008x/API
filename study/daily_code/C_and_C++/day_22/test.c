/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月05日 星期一 08时58分26秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

int main()
{
	int fd[2];
	int ret,request;
	char rbuf[4096];
	char wbuf[5000];
	char *msg="hello,pipe",*msg2="hello,i come here,too!";

	pid_t pid;
	int stat_val;

	if(pipe(fd)<0){
		printf("pipe,error\n");
		exit(1);
	}
	
	//chid process
	if( (pid=fork())==0 ){
	
		close(fd[0]);

		ret=write(fd[1],msg,strlen(msg)+1);
		printf("i am wirte and i write %d byte data\n",ret);
		sleep(5);
		close(fd[1]);
		exit(0);
	//father process	
	}else if(pid>0){
	
		close(fd[1]);
		request=5000;
		ret=read(fd[0],rbuf,request);
		printf("when read request %d byte,the actul return byte is %d\n",request,ret);


		request=10;
		ret=read(fd[0],rbuf,request);
		printf("when read request %d byte,the actul return byte is %d\n",request,ret);
		
		request=100;
		ret=read(fd[0],rbuf,request);
		printf("when read request %d byte,the actul return byte is %d\n",request,ret);
		
		request=1;
		ret=read(fd[0],rbuf,request);
		printf("when read request %d byte,the actul return byte is %d\n",request,ret);
		close(fd[0]);
		exit(0);
	}




	return 0;
}
