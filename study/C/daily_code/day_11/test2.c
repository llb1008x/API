/*************************************************************************
	> File Name: test2.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年05月03日 星期三 13时40分30秒

	字符串操作
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	char str[]="hello";
	char *p=strchr(str,'l');
	char c=*p;
	*p='\0';
	char *t =(char *)malloc(strlen(p)+1);
	strcpy(t,str);
	printf("%s\n",t);
	free(t);

	return 0;
}
