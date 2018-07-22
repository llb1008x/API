
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "key_ioctl.h"

int main(int argc, char *argv[])
{
	int fd = 0;
	int key;
	int retval;

	fd = open("/dev/demo0", O_RDWR | O_NONBLOCK);
	if(fd < 0){
		perror("<user space>: open /dev/demo0");
		goto out;
	}

	/*配置按键的触发方式，即:
	  *   方式一:  (KEY_TRIGGER_LOW)行输出低电平，列设为输入且上拉
	  *   方式二:  (KEY_TRIGGER_HIGH)行输出高电平，列设为输入且下拉
	  */
	retval = ioctl(fd, KEY_TRIGGER_HIGH, 0);
	if(retval < 0){
		perror("<user space>: ioctl syscall");
		goto out;
	}

	while(1){
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


