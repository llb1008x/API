/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年05月18日 星期四 08时27分58秒
 ************************************************************************/

#include<stdio.h>

#define RAD1(x) (x*3.1415)
#define RAD2(x) (x)*3.1415

int main()
{
	printf("RAD1->%f\n",RAD1(1));
	printf("RAD2->%f\n",RAD2(2));

	return 0;
}

