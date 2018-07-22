#include <stdio.h>
#include <string.h>
int main()
{
	char *str1 = "hello world";
	char *str2 = "hello kitty";
	
	if(	strcmp(str1,str2) == 0)
		printf("str1==str2\n");
	else if(strcmp(str1,str2) > 0)
		printf("str1>str2\n");
	else
		printf("str1<str2\n");
		
	if(	strncmp(str1,str2,5) == 0)
		printf("str1==str2\n");
	else if(strcmp(str1,"hello world") > 0)
		printf("str1>str2\n");
	else
		printf("str1<str2\n");
	
	return 0;
}


