#include <stdio.h>
#include <time.h>
#include <windows.h>

typedef struct date
{//表示日期时间的数据类型
	short year;
	char month;
	char day;
	char week_day;
	char hour;
	char minute;
	char second;
}ST_DATE;
	
char *date[7] = { "SUN","MON", "TUES", "WED", "THURS","FRI", "SAT" } ; //星期几

int main()
{
   ST_DATE m_date;

   m_date.year =  10;    
   m_date.month = 1;
   m_date.day = 19;
   m_date.week_day = 4;
   m_date.hour = 10;
   m_date.minute = 00;
   m_date.second = 00;

	printf("当前日期为：\n");
	
	printf("20%02d:%02d:%02d:%s\n",m_date.year,m_date.month,m_date.day,date[m_date.week_day]);
	
	while(1)
	{
		Sleep(1000);
		if(++m_date.second >= 60)
		{
			m_date.second = 0;
			if(++m_date.minute >= 60)
			{
				m_date.minute = 0;
				if(++m_date.hour >= 24)
				{
					m_date.hour = 0;
				}
			}
		}
		printf("%02d:%02d:%02d\r",m_date.hour,m_date.minute,m_date.second);
	}
}