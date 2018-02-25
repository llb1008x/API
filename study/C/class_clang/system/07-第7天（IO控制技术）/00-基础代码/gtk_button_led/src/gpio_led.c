#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "s5pv210-gpio.h"

//硬件连接
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

//led == 1：控制第一个LED
//led == 2：控制第二个LED
//led == 3：控制第三个LED
//flag == 0：灯灭
//flag == 1：灯亮
void led_control(int led, int flag)
{
	int gpio_fd = 0;
	int led_status_old = 0;
	int led_status_new = 0;
	
	gpio_fd = open_dev("/dev/gpH0");

	ioctl(gpio_fd, GPIO_SET_PIN_OUT, 0x1);
	ioctl(gpio_fd, GPIO_SET_PIN_OUT, 0x2);
	ioctl(gpio_fd, GPIO_SET_PIN_OUT, 0x3);
	
	ioctl(gpio_fd, GPIO_SET_PIN_DRV2, 0x1);
	ioctl(gpio_fd, GPIO_SET_PIN_DRV2, 0x2);
	ioctl(gpio_fd, GPIO_SET_PIN_DRV2, 0x3);
	
	read(gpio_fd, &led_status_old, sizeof(led_status_old));
	//printf("led_status_old = %#x\n", led_status_old);
	switch(led)
	{
		case 1:
		case 2:
		case 3:
			if(flag == 0)
			{
				led_status_new = led_status_old & (~(0x01<<led));
			}
			else if(flag == 1)
			{
				led_status_new = led_status_old | (0x01<<led);
			}
			write(gpio_fd, &led_status_new, sizeof(led_status_new));
			break;
	}
	close(gpio_fd);
}
