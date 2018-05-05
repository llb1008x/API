/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月20日 星期三 23时20分49秒
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include "apue.h"


int main(int argc,char *argv[])
{
    int val;

    if(argc!=2){

        err_quit("usage:a.out <descrptor#>");
    }

    if( (val=fcntl(atoi(argv[1]),F_GETFL,0)) < 0 ){

        err_sys("fcntl error for fd %d ",atoi(argv[1]));
    }    

    switch(val&O_ACCMODE){

        case O_RDONLY:
            printf("read only");
            break;
       
        case O_WRONLY:
            printf("write only");
            break;

        case O_RDWR:
            printf("read write");
            break;
        default:
            err_dump("unknown acess mode");

    }

    if(val & O_APPEND){
        
        printf(",append");
    }

    if(val & O_NONBLOCK){

        printf(",noblocking");
    }

    if(val & O_SYNC){
        
        printf(",synchronout writes");
    }
    

   exit(0);
}
