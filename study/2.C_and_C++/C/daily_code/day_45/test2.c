/*************************************************************************
	> File Name: test2.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年08月07日 星期一 19时46分53秒
	
	函数指针：
	一个函数在编译时被分配一个入口地址,这个地址就
称为函数的指针,函数名代表函数的入口地址.
	int (*p)(int ,int);
	p=max;
	c=(*p)(a,b);
 ************************************************************************/

#include<stdio.h>

int max(int a,int b)
{
	
	return (a>b)?a:b;
}

int min(int a,int b)
{
	return (a<b)?a:b;
}

int add(int a,int b)
{
	return a+b;
}

int process(int a,int b,int (*fun)(int a,int b))
{
	int result;
	
	result=(*fun)(x,y);
	return result;
}

int main()
{
	int a,b,result;
	
	printf("enter a and b:");
	scanf("%d %d",&a,&b);
	
	printf("max->");
	result=process(a,b,max);
	printf("%d\n",result);
	
	printf("min->");
	result=process(a,b,min);
	printf("%d\n",result);
	
	printf("sum->");
	result=process(a,b,add);
	printf("%d\n",result);
	

	return 0;
}
