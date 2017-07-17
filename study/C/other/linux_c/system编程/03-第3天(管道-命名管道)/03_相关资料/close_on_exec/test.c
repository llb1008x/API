#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
	int wr_len = write(3, "hello world", 11);
	printf("len==%d\n", wr_len);

	return 0;
}