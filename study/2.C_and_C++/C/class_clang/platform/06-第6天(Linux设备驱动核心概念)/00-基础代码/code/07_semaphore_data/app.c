#include <stdio.h> /* FILE */
#include <fcntl.h>/* O_RDWR */
#include <linux/fb.h>
#include <sys/mman.h>/* PROT_READ,PROT_WRITE,MAP_SHARED */
#include <string.h>/* memset() */
#include <asm/ioctl.h>	/* _IO */

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
	while(1){
		key = 0;
		ret = read(fd,&key,1);
		if(key <= 0)
			continue;
		printf("user:key=%d\n",key);
		sleep(2);
	}

	close(fd);
	return 0;	
}
