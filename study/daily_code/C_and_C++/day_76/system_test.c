/*************************************************************************
	> File Name: system_test.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月12日 星期二 22时47分16秒
 ************************************************************************/

#include <stdio.h>
#include <dirent.h>
#include "./apue.h"

int main(int argc,char *argv[])
{
    DIR *dp;
    struct dirent *dirp;

    if(argc!=2){

        err_quit("usge:ls directory name");
    }

    if((dp=opendir(argv[1]))==NULL){

        err_sys("can't open %s",argv[1]);
    }

    while((dirp=readdir(dp))!=NULL){

        printf("%s\n",dirp->d_name);
    }

    closedir(dp);
    return 0;
}

