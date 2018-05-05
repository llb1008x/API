/*************************************************************************
	> File Name: test3.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年08月07日 星期一 20时14分10秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//这里malloc的内存是局部变量，随着函数的消亡而消亡
void GetMemory1(char *p,int num)
{
	p=(char *)malloc(num);
}

//这里产生的是内存的地址
void GetMemory2(char **p,int num)
{
	*p=(char *)malloc(num);
}

int main()
{
	char *str=NULL;
	
	GetMemory2(&str,100);
	strcpy(str,"hello world");
	printf("%s\n",str);
	free(str);

	return 0;
}
