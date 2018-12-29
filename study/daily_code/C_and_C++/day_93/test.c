/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月28日 星期日 10时57分33秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "apue.h"

#define MAXSIZE    2

int main(int argc,char **argv)
{
    int fd1,fd2;
    int ret;
    char buf[MAXSIZE]="";    

    fd1=open("./work.txt",O_RDONLY,0666);
    if(fd1<0){

        perror("open error!");
    }
    
    fd2=open("./copy.txt",O_RDWR|O_CREAT,0666);
    if(fd2<0){

        perror("open error!");
    }

    
    while( (ret=read(fd1,buf,MAXSIZE))!=0 ){

        ret=write(fd2,buf,MAXSIZE);
        if(ret<0){

            perror("write error!");
        }
    }    

    close(fd1);
    close(fd2);


    return 0;
}

