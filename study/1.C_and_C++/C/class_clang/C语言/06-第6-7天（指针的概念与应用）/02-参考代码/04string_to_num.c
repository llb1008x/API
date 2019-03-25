#include "stdio.h"
int str_to_num(char * str)
{
	char *temp = str;

	int num=0;

	while (*temp != '\0')
	{
		num = num * 10 + (*temp - '0');
		temp++;
	}
		return num;
}




int main(void)
{

	char *string = "123456";
	int num;
	num = str_to_num(string);
	
	printf("the num is %d\n",num);
}
