#include <stdio.h>
#include <conio.h>
int main()
{
	int total = 0,day = 0,month = 0,year = 0,leap = 0;
	printf("please input: year month day\n");
	scanf("%d %d %d",&year,&month,&day);
	switch(month)
	{
		case 1:total=0;break;//31
		case 2:total=31;break;//28
		case 3:total=59;break;//31
		case 4:total=90;break;//30
		case 5:total=120;break;//31
		case 6:total=151;break;//30
		case 7:total=181;break;//31
		case 8:total=212;break;//31
		case 9:total=242;break;//30
		case 10:total=273;break;//31
		case 11:total=303;break;//30
		case 12:total=334;break;//31
		default:printf("error\n");break;
	}
	total=total+day;
	if( month > 2 && ( (year%4 == 0 && year%100 != 0) || (year%400) == 0 ) )
	{
		leap=1;	
	}
	total=total+leap;
	printf("%d:%d:%d is %dth day\n", year, month, day, total);
	getch();
	return 0;
}