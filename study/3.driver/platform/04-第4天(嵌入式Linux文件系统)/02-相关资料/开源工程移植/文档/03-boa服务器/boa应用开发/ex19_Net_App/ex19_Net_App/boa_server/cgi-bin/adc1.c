#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/types.h>
#include <fcntl.h>
#include "s3c2410-adc.h"

int main(void)
{
	int fd;
	int ret;
	int data;
	printf("%s\r\n\r\n","Content-Type:text/html");	
	
	fd = open("/dev/adc/0",O_RDWR);
	if(fd<0)
	   printf("error!");
	ret = ioctl(fd,ADC_SET_CHANNEL,1);
	if(ret<0)
		printf("error\n");
	ret = read(fd,&data,4);
	if(ret<0)
	  printf("error\n");	
	data = data / 11;
	printf("%2d %　", data);
	close(fd);
	return 0;
}
