/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年08月28日 星期一 17时30分31秒
 ************************************************************************/

#include <stdio.h>

#define MAXN 10

//其中n是多项式的阶数，a[]中存储系数，x是给定点。函数须返回多项式f(x)的值。
double f( int n, double a[], double x )
{

	double y;
	
	if(n-1){
	
		y=a[n-1]+x*f(x,a,n-1);
		printf("%dy--->%lf\n",n-1,y);
	}

	return y;
}

int main()
{
    int n, i;
    double a[MAXN], x;
				
    scanf("%d %lf", &n, &x);
    for ( i=0; i<=n; i++ )
        scanf("%lf", &a[i]);
    printf("%.1f\n", f(n, a, x));
    return 0;
}

