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
	flags = fcntl(fd, F_GETFD);	//��ñ�־
	flags |= FD_CLOEXEC;		//�򿪱�־λ
//	flags &= ~FD_CLOEXEC;	 	//�رձ�־λ
	fcntl(fd, F_SETFD, flags);	//���ñ�־
	
	execl("./test", "test", NULL);
	printf("==========error==========\n");
	return 0;
}