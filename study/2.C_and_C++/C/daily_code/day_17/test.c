/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年05月19日 星期五 13时30分42秒
 ************************************************************************/

#include<stdio.h>

int f(int x)
{
	int s=0;

	while(x++ > 0){
	
		printf("x->%d\n",x);		
		s += f(x);
	}

	return (s>1)?s:1;
}

int main()
{
	f(40);

	return 0;
}	
