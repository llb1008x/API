//S5PV210测试程序

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "s5pv210-gpio.h"

//硬件连接
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


int main(int argc, char **argv)
{	int gpio_fd = 0;
	
	gpio_fd = open_dev("/dev/gpH0");
	
	ioctl(gpio_fd, GPIO_SET_PIN_OUT, 0x0);
	ioctl(gpio_fd, GPIO_SET_PIN_OUT, 0x1);
	ioctl(gpio_fd, GPIO_SET_PIN_OUT, 0x2);
	ioctl(gpio_fd, GPIO_SET_PIN_OUT, 0x3);
	
	ioctl(gpio_fd, GPIO_SET_PULL_DIS, 0x0);
	ioctl(gpio_fd, GPIO_SET_PULL_DIS, 0x1);
	ioctl(gpio_fd, GPIO_SET_PULL_DIS, 0x2);
	ioctl(gpio_fd, GPIO_SET_PULL_DIS, 0x3);
	
	ioctl(gpio_fd, GPIO_SET_PIN_DRV2, 0x0);
	ioctl(gpio_fd, GPIO_SET_PIN_DRV2, 0x1);
	ioctl(gpio_fd, GPIO_SET_PIN_DRV2, 0x2);
	ioctl(gpio_fd, GPIO_SET_PIN_DRV2, 0x3);
	
	ioctl(gpio_fd, GPIO_CLR_PIN, 0x0);
	ioctl(gpio_fd, GPIO_CLR_PIN, 0x1);
	ioctl(gpio_fd, GPIO_CLR_PIN, 0x2);
	ioctl(gpio_fd, GPIO_CLR_PIN, 0x3);
	
	char led = 0x02;
	while(1)
	{	
		write(gpio_fd, &led, sizeof(led));
		printf("led=%#x\n",  led);
		led = led<<1;
		if(led == (0x02<<3))
		{
			led = 0x02; 
		}
		usleep(300000);
	}
	close(gpio_fd);
	return 0;
}
