#include <stdio.h>
typedef struct 
{
  unsigned int a:1;
  unsigned int b:2;
  unsigned int c:3;
} M; 

int main()
{
	M k;
	k.a = 1;
	k.b = 1;
	k.c = 4;
	return 0;
} 