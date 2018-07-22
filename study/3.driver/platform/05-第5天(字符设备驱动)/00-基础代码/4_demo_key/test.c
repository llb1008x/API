
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

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
		//�ɹ�����ʵ�ʶ�ȡ�ֽ�����ʧ�ܷ��ظ�ֵ
		retval = read(fd, &key, sizeof(key));
		if(retval < 0){
			perror("<user space>: reading  key value");
			goto out;
		}
		printf("<user space>: read bytes: %d   read content(key =): %d\n", retval, key);
		sleep(1);
	}
	return 0;
out:
	if(fd > 0)
		close(fd);
	return -1;
}


