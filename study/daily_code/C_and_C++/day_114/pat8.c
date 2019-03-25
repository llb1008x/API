/*************************************************************************
	> File Name: pat6.c
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年02月24日 星期日 22时36分37秒
	
6-8 简单阶乘计算 （10 分）
本题要求实现一个计算非负整数阶乘的简单函数。
 ************************************************************************/

#include <stdio.h>

int Factorial( const int N );

int main()
{
    int N, NF;
	
    scanf("%d", &N);
    NF = Factorial(N);
    if (NF)  
        printf("%d! = %d\n", N, NF);
    else 
        printf("Invalid input\n");

    return 0;
}

int Factorial(const int N)
{
    int i,ret=1;
    
    if(N>12 || N<0)
        return 0;
    
    for(i=1;i<=N;i++){
        
        ret = ret*i;
    }    
        
        return ret;
}
