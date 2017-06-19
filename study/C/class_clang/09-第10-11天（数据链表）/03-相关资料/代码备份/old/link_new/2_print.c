//=========================================================================
//	文件名：	2_print.c
//  功能描述：	遍历链表
//	维护记录:	2008-4-17 v1.0		by xdw
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