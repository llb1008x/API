/*************************************************************************
	> File Name: test2.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月20日 星期二 08时40分27秒
	搜索
 ************************************************************************/

#include<stdio.h>
int search(int number,int a[],int len)
{
	int ret=-1,i;
	for(i=0;i<len;i++){
	
		if( number == a[i]){
		
			ret=i;
			break;
		}
	}

	return ret;
}

int main()
{
	int a[]={1,2,3,4,5,6,7};
	int number,ret;

	printf("请输入要搜索的数字: ");
	scanf("%d",&number);

	ret=search(number,a,sizeof(a)/sizeof(a[0]));

	printf("ret->%d\n",ret);

	return 0;
}
