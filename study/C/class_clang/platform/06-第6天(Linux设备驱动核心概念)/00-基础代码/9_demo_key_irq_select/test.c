#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>

int main()
{
	char *devname1 = "/dev/key19_eint";
	char *devname2 = "/dev/key20_eint";

	int fd1,fd2,fd_max;
	unsigned char key;
	int retval;
	
	fd1 = open(devname1, O_RDONLY);
	if (fd1 < 0){
		perror("open key_eint2_poll");
		exit(1);
	}
	
	fd2 = open(devname2, O_RDONLY);
	if (fd2 < 0){
		perror("open key_eint3_poll");
		exit(1);
	}
	
	fd_set rds;
	int ret;
	
	FD_ZERO(&rds);
	FD_SET(fd1, &rds);
	FD_SET(fd2, &rds);
	
	if(fd1 > fd2)
		fd_max = fd1;
	else 
		fd_max = fd2;
		
	while(1){
		
	
		/*使用select监听设备描述符是否有变化*/
		retval = select(fd_max + 1, &rds, NULL,NULL, NULL);
		
		switch(retval){
			case -1 :
				perror("select");
				exit(1); 
				break;
			case 0 :
				printf("Timeout.\n"); 
				break;
			default:
				if (FD_ISSET(fd1, &rds)) /*检查是否有数据可读*/
				{
					ret = read(fd1,&key, sizeof(key));
					printf("the key = %d\n",key);
				}
				else if (FD_ISSET(fd2, &rds)) /*检查是否有数据可读*/
				{
					ret = read(fd2,&key, sizeof(key));
					printf("the key = %d\n",key);
				}	
		}			
	}
	close(fd1);
	close(fd2);
}