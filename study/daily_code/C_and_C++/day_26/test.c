/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月16日 星期五 08时26分57秒
 ************************************************************************/

#include<stdio.h>

int main()
{
	unsigned char c=0xaa;
	printf("c->%hhx\n",c);
	printf("~c->%hhx\n",(char )~c);
	printf("-c->%hhx\n",(char)-c);

	char d=0xaa;
	printf("d->%hhx\n",d);
	return 0;
}
