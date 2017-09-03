/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年08月15日 星期二 08时37分58秒
计算多项式
 ************************************************************************/

#include <stdio.h>
#include <time.h>
#include <math.h>

#define		 MAX	1000000

clock_t start,stop;
//没有CLK_TCK,有类似的CLOCKS_PER_SEC


//1.公式法计算多项式
int fun1(int n,int *a,int x)
{   
	int cout,i;
		
	for(i=0;i<n;i++){
	
		cout += a[i]*pow(x,i);	
	}
	
	return cout;
}

//2.合并多项式法
int fun2(int n,int *a,int x)
{
	int cout,i;
	
	for(i=0;i<n;i++){
	
		cout=a[i]+cout*x;
	}
	
	return cout;
}

int main()
{
#if 	1
	double duration1,duration2;
	int a[MAX],i,cout1,cout2;
	for(i=0;i<MAX;i++){
	
		a[i]=i;
	}

	start=clock();
	cout1=fun2(MAX,a,1.1);
	stop=clock();

	duration1=(double )(stop-start)/CLOCKS_PER_SEC;
	
	start=clock();
	cout2=fun2(MAX,a,1.1);
	stop=clock();
	
	duration2=(double)(stop-start)/CLOCKS_PER_SEC;
	
	printf("duration1->%lf,duration2->%lf\n",duration1,duration2);
	printf("cout1->%d,cout2->%d\n",cout1,cout2);
#endif 


	return 0;
}
