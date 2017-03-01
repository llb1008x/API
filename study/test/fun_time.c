/***************************************
	计算多项式并记录函数运行的时间
***************************************/
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <math.h>


clock_t  start ,stop;

double duration;


/**************************************
	    方案一
**************************************/
int fun(int x,double a[],int n)
{
	double y;
	int i;

	for(i=0;i<n;i++){

		y=y+a[i]*pow(x,i);
		printf("y--->%lf\n",y);
	}
	
	return y;
}


/************************************
		 方案二
************************************/






int main()
{
	double a[]={1,2,3,4,5,6,7,8,9,10,11,12};	
	
	start=clock();	
	fun(1,a,11);
	stop=clock();

	duration=( (double)(stop-start))/CLOCKS_PER_SEC;

	printf("duration--->%lf\n",duration);

	return 0;
}
