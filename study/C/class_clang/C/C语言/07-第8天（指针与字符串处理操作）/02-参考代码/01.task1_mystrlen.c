#include <stdio.h>
#include <string.h>

int my_strlen(const char *str)
{
	int num=0;
	while(*str!='\0')
	{
		str++;
		num++;
	}
	return num;
}
