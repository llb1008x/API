#include <stdio.h>
	float x = 3.6f;
int main()
{
	
	unsigned int i = x;
	unsigned int *p = &x;
	printf("x=%f,i=0x%x,p_x=0x%x\n",x,i,*p);
	return 0;
}
