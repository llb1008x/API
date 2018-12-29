/*************************************************************************
	> File Name: test_clock.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年08月22日 星期二 19时05分30秒
	
2.利用Sleep函数结合结构体，编写一个模拟时钟
#include <windows.h>
Sleep(1);  //1ms

模拟时钟
	
 ************************************************************************/

#include <stdio.h>
#include <unistd.h>

//时钟包含年，月，日，小时，分钟，秒
//year,闰年，平年
//month,12个月
//day,28,29,30,31四种可能
//hours,12小时
//mintue,60分钟
//second
typedef struct time_clock
{
	int year;		
	int month[12];
	int day;
	int hours;
	int minute;
	int second;

}TIME;


int leap_year(int year)
{
	if((year%400==0)||((year%4==0)&&(year%100!=0)))
		return 1;
	else 
		return 0;	
}



int main()
{
	TIME time;
	int is_leap;
	

#if 	1	
//1.初始化
	time.year=2017;
	
	is_leap=leap_year(time.year);
	
	printf("%d\n",is_leap);
	
	//time.month[]={31,28,31,30,31,30,31,31,30,31,30,31};	
	time.day=28;
	time.hours=23;
	time.minute=58;
	time.second=50;
	
	while(1){
	
		sleep(1);
		time.second++;
		if(time.second==60){
		
			time.second=0;
			time.minute++;
			if(time.minute==60){
			
				time.minute=0;
				time.hours++;
				if(time.hours==24){
				
					time.hours=0;
					time.day++;
					//这边要判断月份是几月,其他的判断都很简单，但是月份很重要
				}
			}
		}
		
		printf("%d,,%d——%d:%d:%d\n",time.year,time.day,time.hours,time.minute,time.second);
	}
#endif	

	return 0;
}
