#include <stdio.h> /* FILE */
#include <fcntl.h>/* O_RDWR */
#include <linux/fb.h>
#include <sys/mman.h>/* PROT_READ,PROT_WRITE,MAP_SHARED */
#include <string.h>/* memset() */
#include <asm/ioctl.h>	/* _IO */
#include <linux/input.h>	/* input_event */
#include "driverapp.h"

int main(void)
{
	int fd;
	struct position pos;
	
	fd = open("/dev/bma150",O_RDWR);
	if(fd<0)
	{
		perror("dev bma150 open:");
		return 1;
	}

	sleep(1);
	while(1)
	{
		read(fd,&pos,sizeof(pos));
		printf("x=%d\ty=%d\tz=%d\n",pos.x,pos.y,pos.z);
		usleep(200*1000);
	}
	close(fd);
	return 0;
}

