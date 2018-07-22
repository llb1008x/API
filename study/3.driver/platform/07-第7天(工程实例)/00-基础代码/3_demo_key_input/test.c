
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>
#include <linux/input.h>

int main(int argc, char *argv[])
{
	int fd = 0;
	int retval;
	struct input_event evt;

	//�Զ�д����������ʽ���豸�ļ�
	fd = open("/dev/event0", O_RDWR);
	if(fd < 0){
		perror("<user space>: open /dev/demo0");
		goto out;
	}

	while(1){
		//�ɹ�����ʵ�ʶ�ȡ�ֽ�����ʧ�ܷ��ظ�ֵ
		retval = read(fd, &evt, sizeof(evt));
		if(retval < 0){
			perror("<user space>: reading  key value");
			goto out;
		}
		printf("<user space>: read bytes: %d   key num=%d\tkey value=%d\n", 
				retval, evt.code, evt.value);
	}
	return 0;
out:
	if(fd > 0)
		close(fd);
	return -1;
}


