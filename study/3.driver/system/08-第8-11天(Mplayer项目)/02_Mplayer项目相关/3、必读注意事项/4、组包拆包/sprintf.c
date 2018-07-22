#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc,char *argv[])
{
	char buf[100]="";

	sprintf(buf,"volume %d 1\n",50);
	printf("buf is %s\n",buf);
	
}