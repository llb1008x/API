#include<stdio.h>
#include <conio.h>
int main()
{
	int num = 0;
	char * week[10]={"asd","asdasd","sdfsd"};
	printf("����1��7֮�������:\n");
	scanf("%d",&num);
	
	printf("%s\n",week[num-1]);
	
	getch();
	return 0;
}