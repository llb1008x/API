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

#define DEVICE_NAME  "/dev/DRV2604L"

int main()
{
	printf("hello world\n");

	int fd;
	int ret,val;

	fd = open(DEVICE_NAME, O_RDWR);
	if(fd < 0){
		perror("<user space>: open /dev/DRV2604L");
		goto out;
	}
/*
	ret=read(fd,val,sizeof(val));
	printf("val->%d\n",val);

	val=0x02;
	ret=write(fd,&val,siezof(val));
*/
	printf("ok!\n");

	return 0;

out:
	if(fd > 0)
		close(fd);
	return -1;
}
