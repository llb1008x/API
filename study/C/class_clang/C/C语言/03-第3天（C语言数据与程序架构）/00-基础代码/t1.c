#include <stdio.h>
int main()
{
	int num=5;
	float f = (float)num/2;
    printf("f--->%f\n",f);
    unsigned int *p = &f;
	
	printf("s1=%x\n",*p);
	return 0;
}
