/*************************************************************************
	> File Name: system_test3.c
	> Author: 
	> Mail: 
	> Created Time: 2017年12月13日 星期三 23时54分55秒
 ************************************************************************/

#include<stdio.h>
#include "apue.h"


int main()
{

    #ifdef NAME_MAX

        printf("NAME_MAX is defined to be %d\n",NAME_MAX+0);
    #else
        printf("no symbol for NAME_MAX\n");
    #endif


    #ifdef _PC_NAME_MAX
        pr+pathconf("name_max=",argv[1],_PC_NAME_MAX);
    #else
        printf("no symbol for _PC_NAME_MAX\n");
    #endif


    exit(0);
}
