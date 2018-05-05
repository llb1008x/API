/************************************************************************
	> File Name: swap.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年04月30日 星期日 16时41分47秒
 ************************************************************************/

#include<stdio.h>

void swap(int *u,int *v)
{
	int temp;

	temp=*u;
	*u=*v;
	*v=temp;
}

int main()
{
	int x=5,y=10;

	printf("originally x=%d,y=%d\n",x,y);
	swap(&x,&y);
	printf("now x=%d,y=%d\n",x,y);

	return 0;
}
