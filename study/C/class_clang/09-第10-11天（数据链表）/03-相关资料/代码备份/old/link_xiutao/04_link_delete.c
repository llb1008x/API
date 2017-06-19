#include <stdio.h>
#include <stdlib.h>
#include<string.h>
typedef struct student {
	int num;
	int age;
	struct student * next;
}STU;
STU *creat(int n);

void link_print(STU * head);
STU * link_search(STU *head, int num);
STU * link_delete(STU *head,int num);
int main()
{
	STU * head=NULL;
	STU *pb;
	int num;
	head=creat(3);
	
	link_print(head);
	printf("请输入您要删除的学生的num\n");
	scanf("%d",&num);
	
	head=link_delete(head,num);
	
	link_print(head);
	
	return 0;
}

STU *creat(int n)
{
	int i;
	STU * head,*pb,*pf;
	for(i=0;i<n;i++)
	{
		pb=(STU *)malloc(sizeof(STU));
		printf("please input num  age:\n");
		scanf("%d%d",&(pb->num),&(pb->age));
		getchar();
		if(i==0)
		{
			head=pf=pb;
		}
		else
		{
			pf->next=pb;
			pf=pb;
		}
	}
	pb->next=NULL;
	return head;
}

void link_print(STU * head)
{
	STU *pb;
	pb=head;
	while(pb!=NULL)
	{
		printf("%d %d \n",pb->num,pb->age);
		pb=pb->next;
	}
}

STU * link_search(STU *head, int num)
{
	STU *pb;
	pb=head;
	while(pb!=NULL)
	{
		if(pb->num == num )
			return pb;
		else
			pb=pb->next;
	}
	return NULL;
}

STU * link_delete(STU *head,int num)
{
	STU * pf,*pb;
	pb=pf=head;
	if(head==NULL)// 如果链表为空 返回NULL不需要删
	{
		printf("link is NULL\n");
		return NULL;
	}
	while((pb->num!=num) && (pb->next!=NULL))//循环找pb的num为num的节点
	{
			pf=pb;//pf记录一下pb的位置
			pb=pb->next;//pb指向下一个节点
	}
	if(pb->num==num)//找到了要删除的节点
	{
		if(pb==head)//要删除的节点是头节点
		{
			head=head->next;
		}
		else//要删除的节点是普通节点
		{
			pf->next=pb->next;// pf 指向pb的下一个节点
		}
		free(pb);
	}
	else//没有节点的num为num
	{
		printf("没有您要删除的节点\n");
	}
	return head;
}