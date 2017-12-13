#include <stdio.h>
#include <conio.h>
int main()
{
	int num = 0,i;
	printf("1-100能被7整除的数值为：\n");
	while( num < 100 )
	{
		if( num % 7 == 0 )
			printf("%d ",num);
		num++;
	}
	printf("\n");
	for(i=0;i<=100;i++)
	{
		if( i % 7 == 0 )
			printf("%d ",i);
	}
	printf("\n");
	getch();
	return 0;
}