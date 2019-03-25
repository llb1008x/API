#include <stdio.h>
#include <string.h>
int main()
{
	char str1[20]="hello";
	char *str2 ="hello";
	printf("%d\n",sizeof(str1));
	printf("%d\n",sizeof(str2));
	printf("%d\n",strlen(str1));
	printf("%d\n",strlen(str2));
	return 0;
}
