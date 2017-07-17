#include <stdio.h>
int  main()
{
	int a=0x1234,b=0x5678;
	char *p1,*p2;
	printf("%0x %0x\n",a,b);
	
	p1=(char *)&a;
	p2=(char *)&b;
	printf("%0x %0x\n",*p1,*p2);
	
	p1++;
	p2++;
	printf("%0x %0x\n",*p1,*p2);
	return 0;
}