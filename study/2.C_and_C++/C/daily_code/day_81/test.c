/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月27日 星期三 22时58分42秒
 ************************************************************************/

#include<stdio.h>
#include "apue.h"


int main(int argc,char *argv[])
{
    int i;
    struct stat buf;
    char *ptr;

    for(i=1;i<argc;i++){

        printf("%s :",argv[i]);
        if(stat(argv[i],&buf)<0){

            err_ret("lstat error");
            continue;
        }

        if(S_ISREG(buf.st_mode))
            ptr="regular";
        else if(S_ISDIR(buf.st_mode))
            ptr="directory";
        else if(S_ISCHR(buf.st_mode))
            ptr="character special";
        else if(S_ISFIFO(buf.st_mode))
            ptr="fifo";
        else if(S_ISLNK(buf.st_mode))
            ptr="symbolic link";
        else if(S_ISSOCK(buf.st_mode))
            ptr="socket";
        else
            ptr="** unknown  mode";


        printf("%s \n",ptr);
    }


    return 0;
}
    
