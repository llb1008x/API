/*************************************************************************
	> File Name: test.c
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年02月22日 星期五 23时58分10秒

pat 6-2 计算多项式的和	
本题要求实现一个函数，计算阶数为n，系数为a[0] ... a[n]的多项式f(x)=∑
​i=0​n(a[i]×xi) 在x点的值。

函数接口定义：
double f( int n, double a[], double x );
其中n是多项式的阶数，a[]中存储系数，x是给定点。函数须返回多项式f(x)的值。	
	
输入样例：
2 1.1
1 2.5 -38.7

输出样例：
-43.1
	
 ************************************************************************/

#include<stdio.h>
#include<math.h>

#define MAXN 10

double f( int n, double a[], double x );

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

double f( int n, double a[], double x )
{
    int i;
    double  mutlipy=1.0,sum=0;
    for(i=0;i<=n;i++)  
    {
      sum+=a[i]*mutlipy;
      mutlipy*=x;
      printf("sum->%lf,mutlipy->%lf\n",sum,mutlipy);
    }
    
    /*
        之前错误的版本是 底数不对
        for(i=0;i<=n;i++){
            
            sum = sum + a[i]*x;
        }
        
    */
    
    return sum;
}

