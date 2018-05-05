#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct student{
	int num;
	char array[0];
};

int main()
{
	struct student *p;
	int num;
	
	printf("请输入数组长度\n");
	scanf("%d",&num);
	p = malloc(sizeof(struct student)+num);
	p->num = num;
	scanf("%s",p->array);
	//strcpy(p->array,"hello array!");
	printf("%s\n",p->array);
	printf("%p\n",p->array);
	printf("%p\n",p);
}






