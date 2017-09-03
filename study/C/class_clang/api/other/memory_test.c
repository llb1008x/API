/*************************************************************************
	> File Name: memory_test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年03月12日 星期日 21时39分10秒

	关于程序在各部分在内存中的分布
 ************************************************************************/

#include<stdio.h>

int global=0;

/************************************************************
 *计算矩形的面积
 * *********************************************************/
int rect(int a,int b)
{
	static int count=0;
	int s;

	count++;
	global++;

	s=a*b;

	return s;
}


/**********************************************************
 *计算正方形的面积
 * *******************************************************/
int square(int a)
{
	static int count=0;
	int s;

	count++;
	global++;

	s=rect(a,a);

	return s;
}



int main()
{
	int a=3,b=4;
	int *pa=&a;
	int *pb=&b;
	int *pglobal=&global;
	int (* psquare)(int a)=&square;

	int s=square(a);

	printf("%d\n",s);

	return 0;
}

