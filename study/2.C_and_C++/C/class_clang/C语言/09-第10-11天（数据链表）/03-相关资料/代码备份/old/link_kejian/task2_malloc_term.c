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
	printf("���������������������");
	scanf("%d",&num);

	for( i =0 ;i <num ;i++ )
	{
		ps[i]=(struct stu*)malloc(sizeof(struct stu));		//��ϵͳ������һ��stu���͵Ĵ洢�ռ䣬��������ַ����ps
		printf("�����������ֵܵ���Ϣ��");
		scanf("%s %d",ps[i]->name,&ps[i]->telphone);
	}
	while(1)
	{	
		printf("��������Ҫ��ѯ����ţ�");
		scanf("%d",&key);
		printf("name=%s telphone=%d\n",ps[key-1]->name,&ps[key-1]->telphone);
	}
	for( i = 0; i<num ;i++)
	free(ps[i]);
}