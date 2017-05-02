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
	fd_set rds;
	int fd_max;

	FD_ZERO(&rds);
	FD_SET(fd,&rds);
	fd_max = fd;

	ret = select(fd_max+1,&rds,NULL,NULL,NULL);
	switch(ret){
	case -1:
		perror("select:");
		return -1;
		break;
	case 0:
		printf("timerout\n");
		break;
	default:
		if(FD_ISSET(fd,&rds))
		{
			ret = read(fd,&key_val,sizeof(key_val));
			printf("user:key_status = %s,key_type = %s,key_num = %d,ret=%d\n",key_status[key_val.status],key_type[key_val.type],key_val.num,ret);
					break;
		}
	}
	}
			

	close(fd);
	return 0;	
}
