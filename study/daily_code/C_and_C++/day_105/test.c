/*************************************************************************
	> File Name: test.c
	> Author:llb 
	> Mail:llb1008x@126.com 
	> Created Time: 2018年07月08日 星期日 22时39分11秒
 ************************************************************************/

#include <stdio.h>

void func(int *p)
{
    (*p)++;
}

int main()
{
    int p=0;

    func(&p);
    printf("p->%d\n",p);

    return 0;
}
