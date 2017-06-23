/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月22日 星期四 08时23分52秒

	二分法搜索
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

/*************************************
 *函数：排序
 * **********************************/
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

/*************************************
 *函数：二分搜索
 ************************************/
int search(int a[],int key,int len)
{
	int right=len-1;
	int left=0;
	int mid,ret;

	while(right>=left){
	
		mid=(right+left)/2;
		if(a[mid]==key){
		
			ret=mid;
			break;
		}else if(a[mid]>key){
		
			right=mid-1;
		}else {
		
			left=mid+1;
		}

		printf("left->%d,right->%d,mid->%d\n",left,right,mid);
	}

	if(ret>len)
		return -1;

	return ret;
}


int main()
{
	int a[]={1,5,3,11,82,9,4,73,46,33,37};
	int len=sizeof(a)/sizeof(a[0]);
	int i,ret,number;
	
	printf("输入想要搜索的数字：");
	scanf("%d",&number);

	int *b=(int *)malloc(len);
	b=sort(a,len);
	for(i=0;i<len;i++){
		
		printf("b[%d]->%d\n",i,b[i]);
	}

	ret=search(a,number,len);
	printf("ret->%d\n",ret);


	return 0;
}
