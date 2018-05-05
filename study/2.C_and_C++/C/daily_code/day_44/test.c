/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年08月03日 星期四 20时04分58秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int e;
static int f;
int g=30;
static int h=40;

int main()
{
	int a;
	int b=10;
	static int c;
	static int d=20;
	char *i="test";
	char *k=NULL;
	
	printf("1-a->%d\n",a);
	printf("2-b->%d\n",b);
	printf("3-c->%d\n",c);
	printf("4-d->%d\n",d);
	printf("5-e->%d\n",e);
	printf("6-f->%d\n",f);
	printf("7-g->%d\n",g);
	printf("8-h->%d\n",h);
	printf("9-i->%s\n",i);
	
	k=(char *)malloc(10);
	printf("10-k->%s\n",k);

	return 0;
}
