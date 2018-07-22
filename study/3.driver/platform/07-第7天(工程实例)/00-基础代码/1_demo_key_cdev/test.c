
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>

int main(int argc, char *argv[])
{
	int fd = 0;
	int key;
	int retval;

	//�Զ�д����������ʽ���豸�ļ�
	fd = open("/dev/demo0", O_RDWR | O_NONBLOCK);
	if(fd < 0){
		perror("<user space>: open /dev/demo0");
		goto out;
	}

	while(1){
		fd_set rds;
		int fd_max;

		FD_ZERO(&rds);
		FD_SET(fd, &rds);

		fd_max = fd;		//����ļ�������

		/*ʹ��select�����豸�������Ƿ��б仯*/
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
			printf("<user space>the retval is %d\n",retval);
			if (FD_ISSET(fd, &rds)) /*����Ƿ������ݿɶ�*/
			{
				retval = read(fd, &key, sizeof(key));
				if(retval < 0){
					perror("<user space>: reading  key value");
					goto out;
				}
				printf("<user space>: read bytes: %d   read content(key =): %d\n", retval, key);
			}
		}
	}
	return 0;
out:
	if(fd > 0)
		close(fd);
	return -1;
}


