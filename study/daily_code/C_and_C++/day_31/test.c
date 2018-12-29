/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月21日 星期三 08时18分12秒
 ************************************************************************/

#include<stdio.h>

int amount[]={1,5,10,25,50};
char *name[]={"penny","nickel","dime","quarter","half-dollar"};

int search(int key,int a[],int len)
{
	int ret=-1;
	int i;

	for(i=0;i<len;i++){
		if(key==a[i]){
			
			ret=i;
			break;
		}
	}

	return ret;
}

int main()
{
	int number;
	int ret=-1;

	printf("请输入要搜索的数字：");
	scanf("%d",&number);
	ret=search(number,amount,sizeof(amount)/sizeof(amount[0]));

	if(ret>-1){
		printf("answer->%s\n",name[ret]);
	}
	return 0;
}
