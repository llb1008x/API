#include <string.h>
#include <stdio.h>
int main()
{
	char str1[20]="hello $#$ world";
	char ch='$';
	char str2[20]="$#$";
	char *result;
	
	result=strchr(str1,ch);		
	printf("%s\n",result);
	printf("%d\n",result-str1);
	
	result=strstr(str1,str2);		
	printf("%s\n",result);
	printf("%d\n",result-str1);
	return 0;
}
