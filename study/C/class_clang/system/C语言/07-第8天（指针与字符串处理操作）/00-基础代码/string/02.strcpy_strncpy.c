#include <stdio.h>
#include <string.h>
int main()
{
	char dest1[20]="123456789";
	char dest2[20]="123456789";
	char *src ="hello world";
	
	strcpy(dest1,src);
	printf("%s\n",dest1);	
	
	strncpy(dest2,src,5);
	printf("%s\n",dest2);
	
	dest2[5]='\0';
	printf("%s\n",dest2);
	return 0;
}
