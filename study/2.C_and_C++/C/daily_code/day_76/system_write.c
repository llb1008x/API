/*************************************************************************
	> File Name: system_write.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月12日 星期二 23时47分19秒
 ************************************************************************/

#include<stdio.h>
#include "apue.h"

#define BUFFSIZE    4096

int main(int argc,char argv[])
{
    int n;
    char buf[BUFFSIZE];

    while((n=read(STDIN_FILENO,buf,BUFFSIZE))>0){

        if(write(STDOUT_FILENO,buf,n)!=n){

            err_sys("write error!");
        }
    }

    if(n<0){

        err_sys("read error");
    }


    exit(0);
}

