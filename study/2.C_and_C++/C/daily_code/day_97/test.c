/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2018年02月13日 星期二 20时41分07秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAX_SIZE 1000

int main()
{
    int fd;
    ssize_t length_w,length_r=MAX_SIZE,ret;
    char *testwrite="./testwrite.txt";
    char buffer_write[]="hello write function!";
    char buffer_read[MAX_SIZE];
    
    fd=open(testwrite,O_RDWR|O_CREAT,0777);
    if(fd<0){

        printf("open failed!\n");
    }

    length_w=write(fd,buffer_write,strlen(buffer_write));
    if(length_w==-1){

        perror("write");
    }else {

        printf("write function ok!\n");
    }

    close(fd);

    fd=open(testwrite,O_RDWR|O_CREAT,0777);
    if(fd<0){
        
        printf("open failed !\n");
    }

    ret=read(fd,buffer_read,length_r);
    if(ret){

        perror("read");
    }

    printf("files content is %s \n",buffer_read);
    close(fd);

    return 0;
}

