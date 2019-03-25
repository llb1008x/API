#include <stdio.h>
int main()
{
	int num = 0;
	while(1)
	{
		printf("输入0到7之间的数字:\n");
		scanf("%d",&num);
		if(num == 0)
			break;		//结束本层循环，即推出while
		if(num<0 || num>7)	
		{
			printf("请输入正确的数字\n");
			continue;	//结束本次循环
		}
		switch(num)
		{
			case 1:printf("Today is Monday\n")		;break;
			case 2:printf("Today is Tuesday\n")	;break;
			case 3:printf("Today is Wednesday\n") 	;break;
			case 4:printf("Today is Thursday\n")  	;break;
			case 5:printf("Today is Friday\n")    	;break;
			case 6:printf("Today is Saturday\n")  	;break;
			case 7:printf("Today is Sunday\n")    	;break;
			default :printf("error\n")			;break;
		}
	}
	return 0;
}