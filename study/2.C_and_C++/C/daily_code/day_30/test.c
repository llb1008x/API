/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月20日 星期二 08时23分47秒
 ************************************************************************/

#include<stdio.h>

#define NUM_SIZE  5

//typedef char (*int_array)[10];


int main()
{
	char *a[NUM_SIZE]={"jake","rose","kobe","james","durant"};
	int i;

	for(i=0;i<NUM_SIZE;i++){
	
		printf("a[%d]->%s\n",i,a[i]);
	}

	return 0;
}
