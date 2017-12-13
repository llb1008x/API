#include <stdio.h>

int mystrlen(char *str)
{
	int len = 0;
	char *p = str;
	
	while(*str != '\0')
		str++;
		
	return str-p;
}
char *mystrcpy(char *dest,char *src)
{
	char *str = dest;
	
	// while(*src != '\0'){
		// *str = *src;
		// src++;
		// str++;
	// }	
	// *str = '\0';
	while(*str++ = *src++);
	
	return dest;
}
int myatoi(char *str)//"12345"
{
	int num = 0;
	while(*str != '\0'){
		num = num*10+*str-'0';
		str++;
	}
	
	return num;
}
int mystrcmp(char *str1,char *str2)
{
	while(*str1 || *str2){
		if(*str1>*str2)
			return 1;
		else if(*str1<*str2)
			return -1;
		else{
			str1++;
			str2++;
		}
	}
	if((*str1 == '\0')&&(*str2 == '\0'))
		return 0;
	else
		return (*str1-*str2);
}
int main()
{
	char *str[20];
	printf("len = %d\n",mystrlen("sfsdfsdfsdf"));
	printf("dest = %s\n",mystrcpy(str,"sfsdfsdfsdf"));
	printf("dest = %s\n",str);
	printf("num = %d\n",myatoi("12345"));
	printf("%s\n",mystrcmp("12345","12345")?"不等":"相等");
	return 0;
}
 