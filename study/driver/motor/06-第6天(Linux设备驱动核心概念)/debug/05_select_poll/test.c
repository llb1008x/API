#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	fd_set fds;
	int fd = 0;
	char buf[20] = "";
	
	FD_ZERO(&fds);
	fd = open("/dev/my_zxc0",O_RDWR);
	FD_SET(fd,&fds);
	ioctl(fd,13);
	while(1)
	{	//开始调用驱动中的poll函数
		select(fd+1,&fds,NULL,NULL,NULL);
		printf("under select\n");
		if(FD_ISSET(fd,&fds))
		{
			read(fd,buf,sizeof(buf));
			printf("----%s---\n",buf);
		}
		else
			printf("NO\n",buf);
	}
	close(fd);

	return 0;
}
/*
~_~[root@sunplusedu /home]#./test
zai demo_open
zai my_handle data = key 2
under selectzai my_workhandle 1
*************timer_function data = 97

----key 2---
zai my_handle data = key 2
under selectzai my_workhandle 1
*************timer_function data = 97

----key 2---
*/