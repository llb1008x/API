
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int fd=0;
	unsigned char buf1[100] = "I am a test program!";
	unsigned char buf2[100] = {0};
	int retval;

	fd = open("/dev/demo", O_RDWR);
	if(fd < 0){
		perror("open /dev/demo");
		goto out;
	}

	//成功返回实际写入字节数，失败返回负值
	retval = write(fd, buf1, strlen(buf1)+1);
	if(retval < 0){
		perror("writing fd1 failed!");
		goto out;
	}
	printf("<user space>: write bytes: %d   write content: %s\n", retval, buf1);

	//成功返回实际读取字节数，失败返回负值
	retval = read(fd, buf2, sizeof(buf2));
	if(retval < 0){
		perror("reading fd2 failed!");
		goto out;
	}
	printf("<user space>: read bytes: %d   read content: %s\n", retval, buf2);

	return 0;
out:
	if(fd > 0)
		close(fd);
	return -1;
}


