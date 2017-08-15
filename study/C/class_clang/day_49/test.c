/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年08月15日 星期二 08时37分58秒
计算多项式
 ************************************************************************/

#include <stdio.h>
#include <time.h>

clock_t start,stop;
double duration;

//没有CLK_TCK,有类似的CLOCKS_PER_SEC

int fun1()
{
	printf("hello world\n");
	return 1;
}

int main()
{
	start=clock();
	fun1();
	stop=clock();

	duration=(double )(stop-start)/CLOCKS_PER_SEC;
	printf("duration->%lf\n",duration);

	return 0;
}
