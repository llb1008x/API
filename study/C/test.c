/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年03月22日 星期三 23时47分20秒
	指针的使用，getint函数
 ************************************************************************/

#include<stdio.h>
#include <ctype.h>

void ungetch(int )
{
	
}

/******************************************
 *功能：实现getint函数
  参数：传入字符串的地址
  返回值：输入的是否达到文件末尾
 * ***************************************/
int getint(char *str)
{
	int c,sign;
	//skip white space
	while( (isspace(c=getch())) )
						;

	if( !isdigit(c) && c!=EOF && c!='+' && c!='-' ){
	
			ungetch(c);
			return 0;
	}

	sign=(c == '-')?-1:1;
	if(c == '+' || c == '-')
		c=getch();

	for(*str=0;isdigit(c);c=getch())
			*str=10*(*str)+(c-'0');

	*str *= sign;
	if(c!=EOF)
		ungetch(c);

	return c;
}


int main()
{
	
		
	return 0;
}
