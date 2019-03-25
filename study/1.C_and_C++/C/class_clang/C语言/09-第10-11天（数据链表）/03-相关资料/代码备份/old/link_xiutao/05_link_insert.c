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
STU * link_insert(STU *head,STU *pi);
int main()
{
	STU * head=NULL;
	STU *pb,*pi;
	int num;
	head=creat(3);
	link_print(head);
    printf("请输入要插入节点的 num和age\n");
	pi=(STU *)malloc(sizeof(STU));
	scanf("%d %d",&(pi->num),&(pi->age));
	head=link_insert(head,pi);
	printf("插入之后的链表为:\n");
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



STU * link_insert(STU *head,STU *pi)
{
	STU * pf,*pb;
	pb=pf=head;
	if(head==NULL)// 如果链表为空，则pi即为head
	{
		head=pi;
		return head;
	}

	while((pi->num >= pb->num) && (pb->next!=NULL))//循环找pb的num为num的节点
	{
			pf=pb;//pf记录一下pb的位置
			pb=pb->next;//pb指向下一个节点
	}
	if(pi->num < pb->num)//找到pb的num比pi的num大了,pi 插在pb的前边
	{
		if(pb==head)//插在第一个节点的前边
		{
			pi->next=head;//新来的节点指向 原先的head
			head=pi;//pi 指向的节点 变成头节点
		}
		else//插在普通节点的前边
		{
			pf->next=pi;
			pi->next=pb;
		}
	}
	else//没有找到pb的num比pi的num大，插在链表的尾端 
	{
		pb->next=pi;
		pi->next=NULL;//pi 做为尾节点
	}
		return head;
}
