/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月21日 星期三 08时18分12秒
 ************************************************************************/

#include<stdio.h>
/*
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
*/

struct money{

	int amount;
	char name[20];
}coins[10]={
	{1,"penny"},
	{5,"nickel"},
	{10,"dime"},
	{25,"quarter"},
	{50,"half-dollar"}
};


int main()
{
	int ret =-1,number;
	int i;
	
	printf("请输入要搜索的数字：");
	scanf("%d",&number);
	for(i=0;i<(sizeof(coins)/sizeof(coins[0]));i++){
		//printf("%d\n",i);
		if(number==coins[i].amount){
			printf("answer->%s\n",coins[i].name);
		}
	}
	return 0;
}
