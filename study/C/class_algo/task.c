/*************************************************************************
	> File Name: task.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年03月02日 星期四 23时00分52秒

功能：计算函数多项式	
 ************************************************************************/

#include <stdio.h>
#include <math.h>

#if		1

double fun(int n,double a[],double x)
{
	int i;
	double p=a[0];

	for(i=1;i<=n;i++){
	
		p += (a[i]*pow(x,i));
	}

	return p;
}

#else 

double fun(int n,double a[],double x)
{
	int i;
	double p=a[0];

	for(i=n;i>0;i--){
	
		p = a[i-1]+x*p;
	}

	return p;
}

#endif

int main()
{
	


	return 0;
}
