#include<stdio.h>
//¹²ÓÃÌå
union xx
{
   long int x;
   char y;
   int z;
}a={10};

int main()
{
	a.x=5;
	a.y=6;
	a.z=15;
	printf("%d\n",a.x+a.y); 
	return 0;
}


