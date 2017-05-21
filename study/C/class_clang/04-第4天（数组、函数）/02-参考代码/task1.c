#include <stdio.h>
#include <conio.h>
int main()
{
	int total = 0,day = 0,month = 0,year = 0,leap = 0,i;
	int month_days[]={31,28,31,30,31,30,31,31,30,31,30,31};
	printf("please input: year month day\n");
	scanf("%d %d %d",&year,&month,&day);
	
	for(i=0;i<month-1;i++)
		total +=month_days[i];
	
	total=total+day;
	if( month > 2 && ( (year%4 == 0 && year%100 != 0) || (year%400) == 0 ) )
		leap=1;	
	
	total=total+leap;
	printf("%d:%d:%d is %dth day\n", year, month, day, total);
	getch();
	return 0;
}