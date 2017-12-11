#include <stdio.h>
//×Ö½Ú¶ÔÆë
struct student1
{
	char a;
	int b;
	short c;
}boy1;

#pragma pack (1)
struct student2
{
	char a;
	short b;
	int c;
}boy2;

int main(void)
{
	printf("%d\n",sizeof(boy1) );
	printf("%d\n",sizeof(boy2) );
	return 0;
}
