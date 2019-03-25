/*************************************************************************
	> File Name: pat3.c
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年02月23日 星期六 23时57分18秒
	
6-5 求自定类型元素的最大值 （10 分）
本题要求实现一个函数，求N个集合元素S[]中的最大值，其中集合元素的类型为自定义的ElementType。

函数接口定义：
ElementType Max( ElementType S[], int N );
其中给定集合元素存放在数组S[]中，正整数N是数组元素个数。该函数须返回N个S[]元素中的最大值，
其值也必须是ElementType类型。
 ************************************************************************/

#include <stdio.h>

#define MAXN 10
typedef float ElementType;

ElementType Max( ElementType S[], int N );

int main ()
{
    ElementType S[MAXN];
    int N, i;

    scanf("%d", &N);
    for ( i=0; i<N; i++ )
        scanf("%f", &S[i]);
    printf("%.2f\n", Max(S, N));

    return 0;
}


ElementType Max( ElementType S[], int N )
{
    ElementType max=0;
    int i;
    
    max=S[0];
    for(i=0;i<N;i++){
        if(max<S[i])
            max=S[i];
    }
    
    return max;
}

