#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

char write_fifo_name[]="ltop_fifo";
char read_fifo_name[]="ptol_fifo";
char writer[]="Lucy";
char reader[]="Peter";

int main(int argc, char *argv[])
{
	int write_fd,read_fd;
	mkfifo(write_fifo_name, S_IRUSR|S_IWUSR);	
	mkfifo(read_fifo_name, S_IRUSR|S_IWUSR);

	printf("hello, I am lucy!\n");
	write_fd = open(write_fifo_name,O_WRONLY);
	if(write_fd<0)
	{
		perror("open_w");
		exit(-1);
	}
	read_fd = open(read_fifo_name,O_RDONLY);
	if(read_fd<0)
	{
		perror("open_r");
		exit(-1);
	}

	pid_t pid;
	
	pid = fork();
	if(pid==0)
	{
		while(1)
		{
			char bufw[256] = "";
			
			printf("%s:", writer);
			fflush(stdout);
			bzero(bufw, sizeof(bufw));
			if(fgets(bufw, sizeof(bufw), stdin) == NULL)
				exit(-1);
			bufw[strlen(bufw)-1] = '\0';
			write(write_fd, bufw, strlen(bufw));	
		}
	}
	else
	{
		while(1)
		{
			char bufr[256] = "";
			int ret;
			
			ret = read(read_fd, bufr, sizeof(bufr));
			if (ret <= 0)  
			
			
			  
				exit(-1);
			printf("\r%s:%s\n", reader, bufr);
			printf("%s:", writer);
			fflush(stdout);
		}
	}
	return 0;
}
