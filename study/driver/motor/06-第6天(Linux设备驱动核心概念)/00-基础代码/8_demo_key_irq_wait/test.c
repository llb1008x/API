
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

	//以读写、不阻塞方式打开设备文件
	fd = open("/dev/demo0", O_RDWR);
	if(fd < 0){
		perror("<user space>: open /dev/demo0");
		goto out;
	}

	while(1){
		//成功返回实际读取字节数，失败返回负值
		retval = read(fd, &key, sizeof(key));
		if(retval < 0){
			perror("<user space>: reading  key value");
			goto out;
		}
		
		printf("<user space>: read bytes: %d   read content(key =): %d\n", retval, key);
	}
	return 0;
out:
	if(fd > 0)
		close(fd);
	return -1;
}


