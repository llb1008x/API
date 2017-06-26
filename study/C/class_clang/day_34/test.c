/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月26日 星期一 08时22分10秒
	中国大学慕课网
	编程题一：

		题目说起来很简单，你会读到两个字符串，每个字符串占据一行，每个字符串的
	长度均小于10000字符，而且第一个字符串的长度小于第二个字符串的。你的程序
	“要找出第一个字符串在第二个字符串中出现的位置，输出这些位置，如果找不到，
	则输出-1”。

 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	char str1[10000]="abba";
	char str2[10000]="ababbba abbabbabbabbaacc";
	int len1,len2;
	int index=0,i=0;

	//gets(str1);
	//gets(str2);
	len1=strlen(str1);
	len2=strlen(str2);
	printf("len1->%d,len2->%d\n",len1,len2);
	//第一个字符串比第二个短，两个字符串介于0~10000之间
	if( (len1>len2)||(len1<=0)||(len2>10000)){
	
		printf("error!\n");
		return -1;
	}

	//循环遍历字符串二
	for(index=0;index<len2;index++){
	
		if()
	}

	puts(str1);
	puts(str2);
	
	return 0;
}
