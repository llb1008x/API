/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年06月04日 星期日 10时11分50秒
 ************************************************************************/

#include <stdio.h>

void inplace_swap(int *x,int *y)
{

    *y = *x ^ *y;
    *x = *x ^ *y;
    *y = *x ^ *y;
    printf("x->%d,y->%d\n",*x,*y);
}


void reverse_array(int a[],int cnt)
{
    int first,last;

    for(first=0,last=cnt-1;
        first<last;first++,last--){
           /*
            if( (cnt%2 != 0)&&(first+2==last) ){

                printf("enter this range\n");
                break;
            }  
           */ 
            inplace_swap(&a[first],&a[last]);
        }
}

int main()
{
    int x=1,y=2;
    int i,cnt;
   // printf("x->%d,y->%d\n",x,y);

   // inplace_swap(&x,&y);
   // printf("x->%d,y->%d\n",x,y);
   
    int a[]={1,2,3,4,5,6,7,8,9};
    cnt=sizeof(a)/sizeof(int);
    reverse_array(a,cnt);
    for(i=0;i<cnt;i++){
        
        printf("a[%d]->%d\n",i,a[i]);
    }

    return 0;
}
