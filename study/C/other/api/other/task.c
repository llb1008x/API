/*
 猴子第一天摘下N个桃子，当时就吃了一半，还不过瘾，就又多吃了一个。
 第二天又将剩下的桃子吃掉一半，又多吃了一个。以后每天都吃前一天剩下的一半零一个。
 到第10天在想吃的时候就剩一个桃子了,问第一天共摘下来多少个桃子？并反向打印每天所剩桃子数。
*/

#include <stdio.h>
#include <string.h>

int func(int n)
{
	int num=0;
	
	if(n<=0){
	
		printf("error\n");
		return -1;
	}
	
	if(n==10){
		
		num=1;
	}else{
	
		num= 2*( func(n+1) +1 );

	}
	
	printf("第%d天，吃了%d个桃子\n",n,num);
	
	return num;

}

int main(int argc,char *argv[])
{
	printf("hello world\n");	
	
	func(atoi(argv[1]));

	return 0;
}
