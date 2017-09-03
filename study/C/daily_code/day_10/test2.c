/*************************************************************************
	> File Name: test2.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年05月02日 星期二 09时16分33秒

	这个程序还是不要运行的好，不然电脑会卡死
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int main()
{
	char *p=NULL;
	int cnt=0;

	while( p=(char *)malloc(1024*1024) ){
	
		cnt++;
	}

	printf("申请了%dM空间\n",cnt);
	free(p);


	return 0;
}
