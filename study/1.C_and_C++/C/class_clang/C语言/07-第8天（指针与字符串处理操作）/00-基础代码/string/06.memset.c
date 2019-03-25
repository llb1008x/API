#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{
	char str1[20]="hello $#$ world";
	char *str2=NULL;
	memset(str1,'$',strlen(str1));
	printf("str1=%s\n",str1);
	
	str2=(char *)malloc(10*sizeof(char));
	memset(str2,0,10*sizeof(char));
	free(str2);
	return 0;
}
