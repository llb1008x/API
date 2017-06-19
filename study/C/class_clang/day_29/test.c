/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月19日 星期一 08时48分35秒
 ************************************************************************/

#include<stdio.h>

int main()
{
	int number;
	scanf("%d",&number);
	unsigned mask=1u<<31;
	for(;mask;mask>>=1){
		
		printf("%d,",number&mask?1:0);
	}

	printf("\n");

	return 0;
}

