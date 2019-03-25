/*************************************************************************
	> File Name: test.c
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年01月12日 星期六 19时29分19秒

练习指针
 ************************************************************************/

#include<stdio.h>


void change(int *a,int *b)
{
    int tmp=*a;
    *a=*b;
    *b=tmp;
}

int main()
{
    int a=5;
    int b=2;

    change(&a,&b);

    printf("num a=%d\nnum b=%d\n",a,b);

    return 0;
}
