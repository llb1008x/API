//=========================================================================
//	�ļ�����	2_print.c
//  ����������	��������
//	ά����¼:	2008-4-17 v1.0		by xdw
//=========================================================================
typedef struct stu
{
	int num;
	int age;
	struct stu *next;
}TYPE;

void print(TYPE * head)
{
	TYPE *pb;
	pb=head;
	while(pb!=NULL)
	{
		printf("%d %d \n",pb->num,pb->age);
		pb=pb->next;
	}
}