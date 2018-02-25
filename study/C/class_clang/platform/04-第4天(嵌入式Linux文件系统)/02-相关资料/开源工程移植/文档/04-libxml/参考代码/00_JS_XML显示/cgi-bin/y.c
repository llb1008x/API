#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>// getenv()
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(){
	printf("Content-type: text/xml \n\n");
	char *tem = getenv("QUERY_STRING");
	int fd;
	char buf[1024];
	fd = open("./y.xml",O_RDONLY);
	read(fd, buf, sizeof(buf));
	printf("%s",buf);
	close(fd);
	return 0;
}