#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	int fd = 0;
	char buf[20];
	
	fd = open("/dev/my_zxc0",O_RDWR);
	while(1)
		read(fd,buf,sizeof(buf));
	printf("----%s---\n",buf);
	close(fd);

	return 0;
}