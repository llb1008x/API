#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
	int fd = open("hello.txt", O_CREAT|O_RDWR, 0666);
	if(fd == -1)
	{
		perror("open error");
		return -1;
	}
	printf("fd===%d\n", fd);
	
	int flags;
	flags = fcntl(fd, F_GETFD);	//获得标志
	flags |= FD_CLOEXEC;		//打开标志位
//	flags &= ~FD_CLOEXEC;	 	//关闭标志位
	fcntl(fd, F_SETFD, flags);	//设置标志
	
	execl("./test", "test", NULL);
	printf("==========error==========\n");
	return 0;
}