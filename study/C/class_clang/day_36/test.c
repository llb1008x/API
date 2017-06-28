/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月28日 星期三 08时20分10秒

	枚举 想给一系列常量特殊的意义，不用那些枯燥的数字，也不用定义很多变量

	枚举实际上是int类型的，是有值的,而且也是以证书来做内部计算和外部输出的

 ************************************************************************/

#include<stdio.h>

enum COLOR{RED,YELLOW,GREEN,NumColor};


int main()
{
	int color =-1;
	char *Color[NumColor]={"red","yellow","green"};
	char *colorname=NULL;

	printf("输入你喜欢的颜色:");
	scanf("%d",&color);
	if((color>=0)&&(color<NumColor)){
	
		colorname=Color[color];
	}else {
		colorname="unknown";
	}

	printf("你喜欢的颜色->%s\n",colorname);

	return 0;
}
