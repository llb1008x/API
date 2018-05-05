/*************************************************************************
	> File Name: system_test3.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月13日 星期三 23时54分55秒
 ************************************************************************/

#include<stdio.h>
#include <error.h>
#include <limits.h>
#include "apue.h"

#define     EINVAL      22

static void pr_pathconf(char *mesg,char *path,int name)
{

    long val;
    
    fputs(mesg,stdout);
    int errno=0;
    
    if((val=pathconf(path,name))<0){
    
        if(errno!=0){
        
            if(errno==EINVAL){
            
                fputs("not supported\n",stdout);
            }else{
            
                err_sys("pathconf error,path=%s",path);
            }
        }else {
        
            fputs("(no limit)\n",stdout);
        }
    }else {
    
        
        printf("%ld\n",val);
    }
    
}


int main(int argc,char *argv[])
{

    printf("111\n");
#ifdef NAME_MAX
    printf("222\n");
    printf("NAME_MAX is defined to be %d\n",NAME_MAX+0);
    
#else
    printf("333\n");
    printf("no symbol for NAME_MAX\n");
#endif

    printf("444\n");
#ifdef _PC_NAME_MAX
    printf("555\n");
    pr_pathconf("name_max=",argv[1],_PC_NAME_MAX);
#else
    printf("666\n");
    printf("no symbol for _PC_NAME_MAX\n");
#endif


    exit(0);
}
