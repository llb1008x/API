/*************************************************************************
	> File Name: string_array.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年03月21日 星期二 23时42分31秒

		字符数组
 ************************************************************************/

#include<stdio.h>

int main()
{
	char str1[]="hello";
	char *str2="world";
	char str3[10]="";

	printf("input the value\n");
	scanf("%s",str3);
	printf("str1->%s\n",str1);
	printf("str2->%s\n",str2);
	printf("str3->%s\n",str3);


	return 0;
}

