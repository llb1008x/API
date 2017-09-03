/*************************************************************************
	> File Name: test2.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年09月03日 星期日 16时51分47秒

二分法查找
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int *sort(int a[],int len)
{
	int i,j;
	int tmp;

	for(i=0;i<len;i++){
		for(j=i+1;j<len;j++){
		
			if(a[i]>a[j]){
				tmp=a[i];
				a[i]=a[j];
				a[j]=tmp;
			}
		}
	}

	return a;
}


int find(int a[],int len,int key)
{
    int left=0,right=len-1;
    int ret,mid;

    while(right>=left){
        
        mid=(left+right)/2;
        printf("left->%d,right->%d,mid->%d\n",left,right,mid);
        
        if(key==a[mid]){

            printf("22222222\n");
            ret=mid;
            break;
        }else if(key>a[mid]){

            printf("33333333\n");
            left=mid+1;
        }else {

            printf("444444444\n");
            right=mid-1;
        }
    
    }

    return ret;
}


int main()
{
    int a[]={1,3,5,7,4,8,9,-2,6,0,-4};
    int size=sizeof(a)/sizeof(int );
    int ret,i;
    
    //int *b=(int *)malloc(size);
    int *b=sort(a,size);
    for(i=0;i<size;i++){
        printf("b[%d]->%d\n",i,b[i]);
    }
    ret=find(b,size,9);

    printf("a->%d,ret->%d\n",size,ret);

    //free(b);
    
    return 0;
}
