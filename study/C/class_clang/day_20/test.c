/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年06月01日 星期四 00时02分49秒
 ************************************************************************/

#include <stdio.h>

void inplace_swap(int *x,int *y)
{
   // printf("x->%d,y->%d\n",*x,*y);
    *x = *x ^ *y;
   // printf("x->%d,y->%d\n",*x,*y);
    *y = *x ^ *y;
   // printf("x->%d,y->%d\n",*x,*y);
    *y = *x ^ *y;
   // printf("x->%d,y->%d\n",*x,*y);
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
    int x=1,y=2,i;
    inplace_swap(&x,&y);

    printf("x->%d,y->%d\n",x,y);
    
	int a[]={1,2,3,4};
	reverse_array(a,sizeof(a)/sizeof(int));		
	for(i=0;i<(sizeof(a)/sizeof(int ));i++){
		printf("a[%d]->%d\n",i,a[i]);
	}

    return 0;
}
