#include <stdio.h> /* FILE */
#include <fcntl.h>/* O_RDWR */
#include <stdlib.h>	/* getenv() */

/*
* debug:
* 		echo /mnt/t > /proc/sys/kernel/hotplug
*		hotplug operation
*/

int main(int argc, char *argv[])
{
	int i;
	int fd;
	char *env[6];
	char *pname[] = {"ACTION","DEVNAME","DEVPATH","SUBSYSTEM","FIRMWARE","SEQNUM"};

	fd = open("/dev/console",O_RDWR);/* stdin */
	fd = open("/dev/console",O_RDWR);/* stdout */

	printf("\n");
	for(i=0;i<argc;i++)
		printf("argv[%d] = %s\n",i,argv[i]);
	printf("\n");
	
	env[0] = getenv("ACTION");
	env[1] = getenv("DEVNAME");
	env[2] = getenv("DEVPATH");
	env[3] = getenv("SUBSYSTEM");
	env[4] = getenv("FIRMWARE");
	env[5] = getenv("SEQNUM");

	for(i=0;i<6;i++)
		if(env[i] != NULL)
			printf("%s = %s\n",pname[i],env[i]);
		
	close(0);
	close(1);
	return 0;	
}
