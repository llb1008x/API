#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

//Ó²¼þÁ¬½Ó
//GPH0_0	LED5
//GPH0_1	LED6
//GPH0_2	LED7
//GPH0_3	LED8

static int open_dev(const char *devname)
{
	int fd = -1;
	
	fd = open(devname, O_RDWR);
	if(fd < 0)
	{
		perror("open_dev");
		_exit(-1);
	}
	return fd;
}

int main()
{
	int gpio_fd = 0;
	int key15 = 0x0f;
		
	gpio_fd = open_dev("/dev/s5pv210-key15-simple");
	
	while(1)
	{	
		read(gpio_fd, &key15, sizeof(key15));
		if(key15!=0)
		printf("key15=%d\n",key15);
	}
	close(gpio_fd);
}