#include<stdio.h>
#include"gb2312_ucs2.h"


int main(int argc, char *argv[])
{
	char src[] = "曾经有一段真挚的爱情";
	printf("src==%s===\n", src);

	char utf8[100] = "";
	str_to_utf8(src, utf8);
	printf("utf8----------%s-----\n", utf8);
	
	char utf8_temp[100] = "";
	gb2312_to_utf8(src, utf8_temp);
	printf("utf8_temp======%s=====\n", utf8_temp);
	return 0;
}
