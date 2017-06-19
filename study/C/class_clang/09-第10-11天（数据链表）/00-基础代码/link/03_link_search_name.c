#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct student
{ 					//数据域
	int num;		//学号
	int score;	//分数
	char name[20];
	struct student *next;//指针域
}STU;

void link_creat_head(STU **p_head,STU *p_new)
{
	STU *p_mov=*p_head;
	if(*p_head==NULL)				//当第一次加入链表为空时，head执行p_new
	{
		*p_head=p_new;
		p_new->next=NULL;
	}
	else	//第二次及以后加入链表
	{		
		while(p_mov->next!=NULL)
		{
			p_mov=p_mov->next;	//找到原有链表的最后一个节点
		}	
		p_mov->next=p_new;		//将新申请的节点加入链表
		p_new->next=NULL;
	}
}


void link_print(STU *head)
{
	STU *p_mov;
	p_mov=head;
	while(p_mov!=NULL)
	{
		printf("num=%d score=%d name:%s\n",p_mov->num,p_mov->score,p_mov->name);
		p_mov=p_mov->next;
	}
}
STU * link_search_num(STU *head,int num)
{
	STU *p_mov;
	p_mov=head;
	while(p_mov!=NULL)
	{
		if(p_mov->num == num)//找到了
		{
			return p_mov;
		}
		p_mov=p_mov->next;
	}
	return NULL;//没有找到
}
STU * link_search_name(STU *head,char *name)
{
	STU *p_mov;
	p_mov=head;
	while(p_mov!=NULL)
	{
		if(strcmp(p_mov->name,name)==0)//找到了
		{
			return p_mov;
		}
		p_mov=p_mov->next;
	}
	return NULL;//没有找到
}
int main()
{
	STU *head=NULL,*p_new=NULL,*pb=NULL;
	int num,i;
	char name[20];
	printf("请输入链表初始个数:\n");
	scanf("%d",&num);
	for(i=0;i<num;i++)
	{	
		p_new=(STU*)malloc(sizeof(STU));//申请一个新节点
		printf("请输入学号、分数、名字:\n");	//给新节点赋值
		scanf("%d %d %s",&p_new->num,&p_new->score,p_new->name);
		link_creat_head(&head,p_new);	//将新节点加入链表
	}
	while(1)
	{
		printf("请输入您要查找学生的姓名\n");
		scanf("%s",name);
		pb=link_search_name(head,name);
		if(pb!=NULL)//找到了
		{
			printf("找到了 num=%d score=%d name:%s\n",pb->num,pb->score,pb->name);
		}
		else
		{
			printf("没有找到您要查找的节点\n");
		}
	}
}
