#include <stdio.h>
int max(int x,int y);	//形式1
//int max(int ,int );	//形式2
int main(void)
{
	int a=10,b=20,max_num;
	max_num=max(a,b);
	printf("max_num=%d\n",max_num);
	return 0;
}
int max(int x,int y)
{
	return x>y?x:y;
}
