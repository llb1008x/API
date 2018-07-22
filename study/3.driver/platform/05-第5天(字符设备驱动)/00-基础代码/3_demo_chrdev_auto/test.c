
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int fd1 = 0, fd2 = 0;
	unsigned char buf1[100] = "I am a test program!";
	unsigned char buf2[100] = {0};
	int retval;

	//�Զ�д����������ʽ���豸�ļ�
	fd1 = open("/dev/demo0", O_RDWR | O_NONBLOCK);
	if(fd1 < 0){
		perror("open /dev/demo1");
		goto out;
	}
	//��ֻ����������ʽ���豸�ļ�
	fd2 = open("/dev/demo1", O_RDONLY);
	if(fd2 < 0){
		perror("open /dev/demo2");
		goto out;
	}

	//�ɹ�����ʵ��д���ֽ�����ʧ�ܷ��ظ�ֵ
	retval = write(fd1, buf1, strlen(buf1)+1);
	if(retval < 0){
		perror("writing fd1 failed!");
		goto out;
	}
	printf("<user space>: write bytes: %d   write content: %s\n", retval, buf1);

	//�ɹ�����ʵ�ʶ�ȡ�ֽ�����ʧ�ܷ��ظ�ֵ
	retval = read(fd2, buf2, sizeof(buf2));
	if(retval < 0){
		perror("reading fd2 failed!");
		goto out;
	}
	printf("<user space>: read bytes: %d   read content: %s\n", retval, buf2);

	return 0;
out:
	if(fd1 > 0)
		close(fd1);
	if(fd2 > 0)
		close(fd2);
	return -1;
}


