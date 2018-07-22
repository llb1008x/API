#include <stdio.h> /* FILE */
#include <fcntl.h>/* O_RDWR */
#include <linux/fb.h>
#include <sys/mman.h>/* PROT_READ,PROT_WRITE,MAP_SHARED */
#include <string.h>/* memset() */

int main(int argc, char *argv[])
{
	int fd = 0;

	fd = open("/dev/demo_dev",O_RDWR);
	if(fd < 0)
	{
		perror("/dev/demo_dev:");
		goto open_fail;
	}

	close(fd);
	return 0;	
	
open_fail:	
	return -1;
}
