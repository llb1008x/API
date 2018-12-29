/*************************************************************************
	> File Name: test2.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年04月23日 星期日 23时16分34秒
	

 ************************************************************************/

#include<stdio.h>

int main()
{
	printf("hello world\n");

	float salary;

	printf("\aEnter your desired monthly salary: ");
	printf("$___\b\b\b\b\b\b");
	scanf("%f",&salary);
	printf("\n\t$%.2f a month is %.2f a year.",salary,salary*12.0);
	printf("\rGee!\n"); 

	return 0;
}
