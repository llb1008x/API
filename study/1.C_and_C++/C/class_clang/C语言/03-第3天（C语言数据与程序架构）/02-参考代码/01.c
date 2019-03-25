#include <stdio.h>
#include "leap.h"



flag=leap_year(year);

int leap_year(int year)
{
	int leap;
	if( (year%4==0 && year !=100) || year%400==0 )
		leap=1;
	else
		leap=0;
	return leap;
}

int main(void)
{
	char ch = '!';
	float score = 0;
	printf("请输入学生分数:\n");
	scanf("%f",&score);
	
	ch=score_judge(score);
	
	printf("成绩评定为：%c\n",ch);
	
}