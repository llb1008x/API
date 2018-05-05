#include <stdio.h> /* FILE */
#include <fcntl.h>/* O_RDWR */
#include <linux/fb.h>
#include <sys/mman.h>/* PROT_READ,PROT_WRITE,MAP_SHARED */
#include <string.h>/* memset() */

int main(int argc, char *argv[])
{
	int fd = 0;
	int ret = 0;
	unsigned char key = 0;

	fd = open("/dev/demo_dev",O_RDWR);
	if(fd < 0)
	{
		perror("/dev/demo_dev:");
		goto open_fail;
	}
	while(1){
	ret = read(fd,&key,1);
	if(ret < 0)
		goto read_fail;
	printf("user:key=%#x\n",key);
	sleep(1);
	}
	
	close(fd);
	return 0;	
	
read_fail:
	close(fd);
open_fail:	
	return -1;
}
