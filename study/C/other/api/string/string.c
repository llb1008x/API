/*************************************************************************
	> File Name: string.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年04月25日 星期二 23时24分04秒

	自定义实现字符串常用的函数
	strlen
	strcmp/strncmp
	strcpy/strncpy
	strcat/strncat
	strchr 字符匹配
	strstr 字符串匹配
	memset 空间设定
	atoi/atol/atof 字符串转换 
	strtok字符串切割  比较有困难的应该是这个
	
char *strtok(char s[], const char *delim);
功能:strtok()用来将字符串分割成一个个片段。
	参数1:s指向欲分割的字符串
	参数2:delim则为分割字符串中包含的所有字符。
	当strtok()在参数s的字符串中发现参数delim中包含
	的分割字符时,则会将该字符改为\0 字符,当连续出现多个
	时只替换第一个为\0。
	在第一次调用时:strtok()必需给予参数s字符串
	往后的调用则将参数s设置成NULL,每次调用成功则返
	回指向被分割出片段的指针

	
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
