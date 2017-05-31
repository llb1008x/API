/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年06月01日 星期四 00时02分49秒
 ************************************************************************/

#include <stdio.h>

void inplace_swap(int *x,int *y)
{
    printf("x->%d,y->%d\n",*x,*y);
    *x = *x ^ *y;
    printf("x->%d,y->%d\n",*x,*y);
    *y = *x ^ *y;
    printf("x->%d,y->%d\n",*x,*y);
    *y = *x ^ *y;
    printf("x->%d,y->%d\n",*x,*y);
}

void reverse_array(int a[],int cnt)
{
    int first ,last;

    for(first=0,last=cnt-1;first<=last;first++,last--){

        inplace_swap(&a[first],&a[last]);
    }
}


int main()
{
    int x=1,y=2;
    inplace_swap(&x,&y);

    printf("x->%d,y->%d\n",x,y);
    
    return 0;
}
