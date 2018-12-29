/*************************************************************************
	> File Name: test2.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月20日 星期三 23时59分13秒
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include "apue.h"


void set_fl(int fd,int flags)
{

    int val;

    if( (val=fcntl(fd,F_GETL,0)) < 0){

        err_sys("fcntl F_GETL error");
    }

    val |= flags;

    if( fcntl(fd,F_SETL,val) < 0 ){

        err_sys("fcntl F_SETFL error");
    }
}    

int main()
{
    

    exit(0);
}
