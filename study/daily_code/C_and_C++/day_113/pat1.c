/*************************************************************************
	> File Name: pat1.c
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年02月23日 星期六 23时55分22秒	
本题要求实现一个函数，对给定的正整数N，打印从1到N的全部正整数。

函数接口定义：
void PrintN ( int N );
其中N是用户传入的参数。该函数必须将从1到N的全部正整数顺序打印出来，每个数字占1行。
 ************************************************************************/

#include<stdio.h>

 
void PrintN ( int N );

int main ()
{
    int N;

    scanf("%d", &N);
    PrintN( N );

    return 0;
}

void PrintN(int N)
{
	int i;

	for(i=1;i<=N;i++){
	
		printf("%d\n",i);
	}

}
