/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年09月03日 星期日 09时28分22秒

    计算多项式的值并利用time.h中的时钟来统计运行时间
 ************************************************************************/

#include <stdio.h>
#include <time.h>
#include <math.h>

#define MAX_SIZE  10
#define MAXK      1e7


//统计计算所用的时间
clock_t  start ,stop;
double duration;


//计算多项式在给定点的值
//n:阶数，a：多项式的系数，x：给定的点
double fun1(int n,double a[],double x)
{
    double sum=0;
    int i;

    sum = a[0];
    for(i=1;i<n;i++){

        sum += a[i]*pow(x,n);
        printf("i->%d,sum->%lf\n",i,sum);
    }

    return sum;
}

double fun2(int n,double a[],double x)
{
    double  sum=0;
    int i;

    for(i=n;i>0;i--){
        
        sum = a[i]+sum*x;
        printf("i->%d,sum->%lf\n",i,sum);
    }
    
    return sum;
}

double work_time(double a[],double x,double (*p)(int n,double a[],double x))
{
    double duration;
    int i=0;

    start=clock();
    for(i=0;i<MAXK;i++)
        p(MAX_SIZE,a,x);
    stop=clock();

    duration=(double )(stop-start)/CLOCKS_PER_SEC/MAXK;

    return duration;
}

int main()
{
 // 1,初始化   
    double a[MAX_SIZE];
    double duration;
    int i,x;

    scanf("%d",&x);
    for(i=0;i<MAX_SIZE;i++)
        a[i]=(double )i;

    duration=work_time(a,x,fun2);
    printf("duration->%lf\n",duration);

// 2.计算函数运行的时间
#if     0   
    start=clock();
    fun2(MAX_SIZE,a,x);
    stop=clock();
   
    duration=10000*(double )(stop-start)/CLOCKS_PER_SEC;

    printf("start->%d,stop->%d,duartion->%lf\n",start,stop,duration);
#endif
    return 0;
}
