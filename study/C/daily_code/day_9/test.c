/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年05月01日 星期一 20时45分04秒

	数组传参
	数组变量事const指针，所以不能被赋值
	int a[],b[];
	a ?= b
 ************************************************************************/

#include<stdio.h>


void minmax(int a[],int len,int *max,int *min)
{
	int i;
	*min=*max=a[0];

	printf("minmax a->%p\n",a);
	printf("minmax sizeof(a)->%ld\n",sizeof(a));
	for(i=1;i<len;i++){
	
		if(a[i]<*min){
			*min=a[i];
		}

		if(a[i]>*max){
		
			*max=a[i];
		}
	}

	a[0]=1000;

}

int main()
{
	printf("hello world\n");
	
	int a[]={1,2,3,4,5,6,7,8,9};
	int min,max;

	printf("main a->%p\n",a);
	printf("main sizeof(a)->%ld\n",sizeof(a));
	minmax(a,sizeof(a)/sizeof(a[0]),&max,&min);
	printf("min=%d,max=%d\n",min,max);
	printf("a[0]->%d\n",a[0]);

	int *p=&min;
	printf("*p->%d\n",*p);
	printf("p[0]->%d\n",p[0]);


	return 0;
}

