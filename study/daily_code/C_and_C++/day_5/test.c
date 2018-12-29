/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年04月25日 星期二 23时18分32秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>

#define PRAISE "what a super marvelous name!"


int main()
{
	char name[40];

	printf("what's your name?\n");
	scanf("%s",name);

	printf("hello ,%s .%s \n",name,PRAISE);
	printf("your name of %d letters occupies %d memory cells.\n",
						strlen(name),sizeof(name));

	printf("the phrase of praise has %d letters ",strlen(PRAISE));
	printf("and occupies %d memory cells .\n",sizeof(PRAISE));

	return 0;
}
