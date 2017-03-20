/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年03月12日 星期日 20时17分34秒

		gdb 工具的使用
 ************************************************************************/

#include<stdio.h>

/***************************
 *交换数值
 * *************************/
void change(int *a,int *b)
{
	int tmp=*a;
	*a=*b;
	*b=tmp;
}

void test()
{

	printf("this is just a test\n");
}

int main()
{
	printf("hello world\n");

	int a=3,b=5;

	change(&a,&b);

	printf("a->%d,b->%d\n",a,b);

	test();

	return 0;
}
