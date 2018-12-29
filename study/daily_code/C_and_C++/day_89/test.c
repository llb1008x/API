/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月22日 星期一 22时20分37秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
//#include "apue.h"

extern char **environ;

int main()
{
    char *ptr;
    
    ptr=getenv("PATH");
    printf("ptr->%s\n",ptr);
    
    
    char **env=environ;
    while(*env){
    
        printf("%s\n",*env);
        env++;
    }

    return 0;
}
