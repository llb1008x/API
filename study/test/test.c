/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年03月10日 星期五 16时58分30秒

	输入一个数，输出他的逆序数
 ************************************************************************/

#include<stdio.h>


int main()
{
	int a,b;
	int num[3];

	scanf("%d",&a);

	if( a>999 ){

		printf("error\n");
		return -1;
	}

	num[0]=a/100;
	num[1]=a%100/10;
	num[2]=a%10;
	/*数值位数逆序*/
	b=num[0]+num[1]*10+num[2]*100;
	printf("b->%d\n",b);

	return 0;
}
