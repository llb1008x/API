/*************************************************************************
	> File Name: read.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月17日 星期三 15时34分09秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "apue.h"


#define BUFFER_SIZE 	1024

int main(int argc, char *argv[])
{
	int fd;
	
	if(argc<2){
	
		printf("usage %s filename\n",argv[0]);
		return -1;
	}
	
	if((fd=open(argv[1],O_RDONLY))<0){
	
		perror("open error");
		exit(-1);
	}
	
	printf("open fifo %s for reading success\n",argv[0]);
	char buffer[BUFFER_SIZE];
	ssize_t n;
	
	while(1){
	
		if((n=read(fd,buffer,BUFFER_SIZE))== -1){
		
			perror("read fail");
			return -1;
		}else if(n==0){
		
			perror("read fail");
			return -1;
		}else {
		
			buffer[n]='\0';
			printf("read %d bytes from fifo %s \n",n,buffer);
		}
		
	}
	

    exit(0);
}

