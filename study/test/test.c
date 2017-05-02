/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年05月02日 星期二 17时34分30秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main()
{
	char *buff=NULL;
	int i=0,n=3;

	buff=(char *)malloc(n);
	buff[0]='1';
	buff[1]='2';
	buff[2]='3';

	for(i=0;i<n;i++){
	
		printf("buff[%d]=%c\n",i,buff[i]);
	}

	free(buff);

	return 0;
}
