#include<stdio.h>
#include<asm/hardware.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>

#include"led_driver.h"

int main()
{
	int i = 0x01;
	int fd;
	fd = open("/dev/gpf/0", O_RDWR);
	if(fd == 0)
	{
		perror("fail: open /dev/gpf0\n");
		exit(1);
	}
	ioctl(fd, GPIO_SET_MULTI_PIN_OUT, 0xff);
	while(1)
	{
		write(fd, &i, sizeof(i));
		sleep(1);
		i = (i << 1);
		if(i & 0x100)
			i =0x01;
	}
	close(fd);
	return 0;
}
	
