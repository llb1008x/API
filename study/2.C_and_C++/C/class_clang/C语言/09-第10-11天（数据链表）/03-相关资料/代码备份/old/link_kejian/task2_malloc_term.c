#include <stdio.h>
#include "stdlib.h"

struct stu
{    int num;
	char name[10];
	int telphone;
}*ps[100];

int main(void)
{
	int num,key,i;
	printf("请输入你们宿舍的人数：");
	scanf("%d",&num);

	for( i =0 ;i <num ;i++ )
	{
		ps[i]=(struct stu*)malloc(sizeof(struct stu));		//在系统中申请一个stu类型的存储空间，并把其首址付给ps
		printf("请输入宿舍兄弟的信息：");
		scanf("%s %d",ps[i]->name,&ps[i]->telphone);
	}
	while(1)
	{	
		printf("请输入你要查询的序号：");
		scanf("%d",&key);
		printf("name=%s telphone=%d\n",ps[key-1]->name,&ps[key-1]->telphone);
	}
	for( i = 0; i<num ;i++)
	free(ps[i]);
}