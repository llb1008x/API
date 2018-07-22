#include <stdio.h>
int main()
{	
	char a1 = '*';
	char a2[20] = "abcdef";
	unsigned short int b1 = 0x5a;
	signed int b2 = -30;
	signed long int b3 = 040;
	float c = 3.14f;
	double d = 3.1415926535898;
	double e = 314e-2;
	printf("a1=%c,%d\n",a1,sizeof(a1));
	printf("a2=%s,%d\n",a2,sizeof(a2));
	printf("b1=%x,%d\n",b1,sizeof(b1));
	printf("b2=%-010d,%d\n",b2,sizeof(b2));
	printf("b3=%o,%d\n",b3,sizeof(b3));
	printf("c=%.2f,%d\n",c,sizeof(c));
	printf("d=%.10f,%d\n",d,sizeof(d));
	printf("e=%.3f,%d\n",e,sizeof(e));
	return 0;
}



