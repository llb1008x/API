/*************************************************************************
	> File Name: test2.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月14日 星期四 20时12分31秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "apue.h"


char buf1[]="abcdefg";
char buf2[]="ABCDEFG";

#define BUFFSIZE	8192

int main()
{
/*
	if(lseek(STDIN_FILENO,0,SEEK_CUR) == -1){
		printf("cannot seek\n");
	}else{
		printf("seek ok\n");
	}
-------------------------
	int fd;
	
	if( (fd=creat("file.hole",FILE_MODE))<0){
	
		err_sys("creat error");
	}
	
	if(write(fd,buf1,10)!=10)
		err_sys("buf1 write error");
		
	if(lseek(fd,40,SEEK_SET)== -1)
		err_sys("lseek error");
		
	if(write(fd,buf2,10)!=10)
		err("buf2 write error");
		
*/
			
	int n;
	char buf[BUFFSIZE];
	
	while( (n=read(STDIN_FILENO,buf,BUFFSIZE))>0){
		if(write(STDOUT_FILENO,buf,n)!=n){
			
			err_sys("write error");
		}
	}	
				
	if(n<0){
		err_sys("read error");
	}			

    exit(0);
}
