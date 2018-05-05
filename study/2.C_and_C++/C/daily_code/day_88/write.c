/*************************************************************************
	> File Name: write.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月17日 星期三 15时33分39秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "apue.h"

#define BUFFER_SIZE	1024

int main(int argc,char *argv[])
{
	int fd;
	
	if(argc<2){
	
		printf("usage %s filename\n",argv[0]);
		return -1;
	}
	
	if((fd=open(argv[1],O_WRONLY))<0){
	
		perror("open error ");
		exit(-1);
	}
	
	printf("open fifo %s for writing success\n",argv[0]);
	
	char buffer[BUFFER_SIZE];
	
	ssize_t n;
	
	while(fgets(buffer,BUFFER_SIZE,stdin)){
	
		if((n=write(fd,buffer,strlen(buffer)))== -1){
			
			perror("write fails");
			break;
		}
	}

    exit(0);
}
