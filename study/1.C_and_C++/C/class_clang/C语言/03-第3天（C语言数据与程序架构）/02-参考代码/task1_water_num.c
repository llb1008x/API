#include <stdio.h>
#include <conio.h>
int main() 
{
	int a,b,c,i;  
	for(i=0;i<=999;i++) 
	{
		a=(i/100)%10; 
		b=(i/10)%10; 
		c=(i/1)%10;  
		if(i==a*a*a+b*b*b+c*c*c) 
			printf("%d  ",i);
	}
	getch();
	return 0;
} 
