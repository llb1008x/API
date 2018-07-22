#include <stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <string.h>
int main(int argc,char *argv[])
{
	int src_fd,dest_fd;
	char buf[100];
	int len;
	src_fd=open(argv[1],O_RDONLY);
	if(src_fd<0) 
	{
		printf("Cannot open the file\n");
		return 0;
	}
	dest_fd=open(argv[2],O_WRONLY|O_CREAT,0666);
	if(dest_fd<0)
	{
		printf("Cannot open the file\n");
		return 0;
	}
	
	while(1)
	{
		memset(buf,0,100);
		len=read(src_fd,buf,100);
		if(len>0)
			write(dest_fd,buf,len);	
		if(len!=100)
			break;
	}
	close(src_fd);
	close(dest_fd);
	return 0;
}
