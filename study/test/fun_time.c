/***************************************
	计算多项式并记录函数运行的时间
***************************************/
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define  NUM   100000


clock_t  start ,stop;

double duration;


#if   0
/**************************************
	    方案一
f=a+b*x+c*x^2+d*x^3.....		
**************************************/
int fun(double x,double a[],int n)
{
	double y;
	int i;

	for(i=0;i<n;i++){

		y=y+a[i]*pow(x,i);
		printf("y--->%lf\n",y);
	}
	
	return y;
}


#else 
/************************************
		 方案二
f=a+x(b+x(c+x(d+.... )))		 
************************************/
int fun(double x,double a[],int n)
{
	double y;
	
	if(n-1){
	
		y=a[n-1]+x*fun(x,a,n-1);
		printf("%dy--->%lf\n",n-1,y);
	}

	
	return y;
}

#endif 


int main()
{
	double a[NUM];
	int i;

	for(i=0;i<NUM-1;i++){
	
		a[i]=i;
	}

	
	start=clock();	
	fun(1.0,a,NUM-1);
	stop=clock();

	duration=( (double)(stop-start))/CLOCKS_PER_SEC;

	printf("duration--->%lf\n",duration);

	return 0;
}
