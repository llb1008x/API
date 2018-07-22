#include <stdio.h> /* FILE */
#include <fcntl.h>/* O_RDWR */
#include <linux/fb.h>
#include <sys/mman.h>/* PROT_READ,PROT_WRITE,MAP_SHARED */
#include <string.h>/* memset() */
#include <asm/ioctl.h>	/* _IO */

#define GPH2_7  _IO('g',7)
#define GPH2_6	_IO('g',6)
#define GPH2_5	_IO('g',5)
#define GPH2_4	_IO('g',4)

/* from kernel */
#define S3C_GPIO_SPECIAL_MARK	(0xfffffff0)
#define S3C_GPIO_SPECIAL(x)	(S3C_GPIO_SPECIAL_MARK | (x))

/* Defines for generic pin configurations */
#define S3C_GPIO_INPUT	(S3C_GPIO_SPECIAL(0))
#define S3C_GPIO_OUTPUT	(S3C_GPIO_SPECIAL(1))
#define S3C_GPIO_SFN(x)	(S3C_GPIO_SPECIAL(x))



int main(int argc, char *argv[])
{
	int fd = 0;
	int ret = 0;
	unsigned char key = 0;

	fd = open("/dev/demo_dev",O_RDWR);
	if(fd < 0)
	{
		perror("/dev/demo_dev:");
		return -1;
	}
	ret = ioctl(fd,GPH2_7,S3C_GPIO_INPUT);
	ret = ioctl(fd,GPH2_6,S3C_GPIO_INPUT);
	ret = ioctl(fd,GPH2_5,S3C_GPIO_INPUT);
	ret = ioctl(fd,GPH2_4,S3C_GPIO_INPUT);
	if(ret < 0){
		close(fd);
		return -1;
	}

	while(1){
		ret = read(fd,&key,1);
		if(key != 0xf)
			printf("user:key=%#x\n",key);
		usleep(1000*100);
	}
	
	close(fd);
	return 0;	
}
