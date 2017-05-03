/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年05月03日 星期三 08时37分37秒

	简单的字符串操作
	指针与数组，二级指针与字符串数组
	EOF->windows:ctrl+z unix：ctrl+D
 ************************************************************************/

#include<stdio.h>

int main()
{
	int ch;

	while( (ch=getchar())!=EOF	){
	
		putchar(ch);
	}

	printf("%d\n",ch);

	return 0;
}
