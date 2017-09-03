/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: Thu 04 May 2017 10:44:46 PM CST
	
	枚举的使用
 ************************************************************************/

#include<stdio.h>

int main()
{
	enum color {red ,yellow ,blue ,black,green};
	printf("red->%d\n",red);

	red=3;
	printf("red->%d\n",red);
	

	return 0;
}
