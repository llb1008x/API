/*************************************************************************
	> File Name: string.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年04月25日 星期二 23时24分04秒

	自定义实现字符串常用的函数
 ************************************************************************/

#include<stdio.h>

/*******************************************
 *函数：my_strlen，统计字符串的个数
 *参数：str字符串
 *返回值：字符串的个数
 * ****************************************/
int my_strlen(char *str)
{
	int num=0;

	if( str==NULL )
		return 0;


	while( *(str++) != '\0' )
		num++;

	return num;
}


int main()
{
	char str[20]="";
	int num;

	scanf("%s",str);
	printf("%s\n",str);

	num=my_strlen(str);
	printf("sizeof str->%d\n",num);

	return 0;
}
