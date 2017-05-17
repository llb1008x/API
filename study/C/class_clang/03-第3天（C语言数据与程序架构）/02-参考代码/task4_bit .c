#include <stdio.h>
#include <conio.h>
int main() 
{  
	unsigned char a = 0x55;

	printf("a1=%x\n",a);
	a |=   (1<<1) | (1<<5) | (1<<7);

	a &=  ~( (1<<2) | (1<<3) | (1<<6) );

	printf("a2=%x\n",a);
	getch();
	return 0;
} 
