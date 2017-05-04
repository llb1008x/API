/*************************************************************************
	> File Name: motor_test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年04月26日 星期三 19时32分17秒
功能：调用马达的测试程序
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include<sys/stat.h>

#define DEVICE_NAME  "/dev/DRV2604L"

int main()
{
	printf("hello world\n");

	int fd;
	int ret=0,value=0,bytes=0,cmd;
	unsigned long arg;
	char buf[20]="";

	fd = open(DEVICE_NAME, O_RDWR);
	if(fd < 0){
		perror("<user space>: open /dev/DRV2604L");
		goto out;
	}

	// test read DRV2604L spot
	ret=read(fd,buf,sizeof(buf));
	printf("buf->%s,ret->%d\n",buf,ret);
	
	while(1){
		
		// test write DRV2604L spot
		/*
		scanf("%d",&value);
		bytes = sprintf(buf,"%d\n",value);
		ret=write(fd,buf,bytes);
		printf("bytes->%d,value->%d,buf->%s\n",bytes,value,buf);
		*/
		
		// test ioctl DRV2604L spot
		scanf("%d %ld",&cmd,&arg);
		ret=ioctl(fd,cmd,arg);
		printf("cmd->%d,arg->%ld,ret->%d\n",cmd,arg,ret);
		
	}

	
/*
	val=0x02;
	ret=write(fd,&val,siezof(val));
*/
	printf("ok!\n");
	
	close(fd);

	return 0;

out:
	if(fd > 0)
		close(fd);
	return -1;
}
