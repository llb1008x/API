#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>// getenv()
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	printf("Content-type: text/xml \n\n");
	int fd;
	char buf[1024] = "";
//	printf("hello");
	 fd = open("./student.xml",O_RDONLY);
	 if(fd<0)
	 {
		printf("open error\n");
		return -1;
	 }
	 read(fd, buf, sizeof(buf));
	 printf("%s",buf);
	 close(fd);
	return 0;
}