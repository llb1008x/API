#include <stdio.h> /* FILE */
#include <fcntl.h>/* O_RDWR */
#include <linux/fb.h>
#include <sys/mman.h>/* PROT_READ,PROT_WRITE,MAP_SHARED */
#include <string.h>/* memset() */
#include <asm/ioctl.h>	/* _IO */

struct key_data{
char status;/* up=0,down=1 */
char type;/* short=0 long=1 */
char num;
};

int main(int argc, char *argv[])
{
	int fd = 0;
	int ret = 0;
	struct key_data key_val;
	char *key_status[] = {"up","down"};
	char *key_type[] = {"short","long"};

	fd = open("/dev/demo_dev",O_RDWR);
	if(fd < 0)
	{
		perror("/dev/demo_dev:");
		return -1;
	}
	while(1){
		ret = read(fd,&key_val,sizeof(key_val));
		if(ret <= 0)
			continue;
		printf("user:key_status = %s,key_type = %s,key_num = %d,ret=%d\n",key_status[key_val.status],key_type[key_val.type],key_val.num,ret);
	}

	close(fd);
	return 0;	
}
