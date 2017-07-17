#include <stdio.h>
#include <string.h>
int main()
{
	char str1[20]="123";
	char str2[20]="123";
	
	char *src ="hello world";
	printf("%s\n",strcat(str1,src));
	printf("%s\n",strncat(str2,src,5));
	return 0;
}



