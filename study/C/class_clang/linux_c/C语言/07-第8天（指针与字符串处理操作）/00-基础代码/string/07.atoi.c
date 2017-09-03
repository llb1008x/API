#include <stdio.h>
#include <stdlib.h>
int main() 
{
	char str1[]="12345",str2[]="56789",str3[]="123.456";
	int num1;
	long num2;
	double num3;
	num1=atoi(str1);
	num2=atol(str2); 
	num3=atof(str3);
	printf("num1=%d,num2=%d,num3=%lf\n",num1,num2,num3);
	return 0;
}
