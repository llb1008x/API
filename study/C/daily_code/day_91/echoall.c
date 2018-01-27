/*************************************************************************
	> File Name: echoall.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月26日 星期五 17时47分12秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"

int main(int argc,char **argv)
{
	int i;
	char **ptr;
	extern char **environ;
	
	for(i=0;i<argc;i++){
	
		printf("argv[%d]: %s\n",i,argv[i]);
	}
	
	for(ptr=environ;*ptr!=0;ptr++){
	
		printf("%s\n",*ptr);
	}
	
	return 0;
}

