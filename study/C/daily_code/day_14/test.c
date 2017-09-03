/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年05月16日 星期二 13时30分49秒
 ************************************************************************/

#include <stdio.h>

int *f(void);

void g(void);

int main()
{
	int *p=f();
	printf("*p->%d\n",*p);
	g();
	printf("*p->%d\n",*p);

	return 0;
}


int *f(void)
{
	int i=12;
	printf("pi->%p\n",&i);
	return &i;
}

void g(void)
{
	int k=24;
	printf("pk->%p\n",&k);
	printf("k->%d\n",k);

}
