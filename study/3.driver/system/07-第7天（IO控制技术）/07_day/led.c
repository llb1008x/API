#include<stdio.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include"s5pv210-gpio.h"
#include<fcntl.h>
int main()
{
	int fd;
	char led;
	fd=open("/dev/gpH0",O_RDWR);
	if(fd<0)
	{
		perror("open");
		return 0;
	}
	
	ioctl(fd,GPIO_SET_PIN_OUT,1);
	ioctl(fd,GPIO_SET_PIN_OUT,2);
	ioctl(fd,GPIO_SET_PIN_OUT,3);
	while(1)
	{
		led=0x02;
		write(fd,&led,1);
		sleep(1);
		led=0x00;
		write(fd,&led,1);
		sleep(1);
	}
	return 0;
}