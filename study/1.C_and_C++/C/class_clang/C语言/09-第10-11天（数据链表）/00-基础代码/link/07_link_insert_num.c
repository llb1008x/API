#include <stdio.h>
#include <stdlib.h>
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
void link_insert_num(STU **p_head,STU *p_new)
{
	STU *pb,*pf;
	pb=pf=*p_head;
	if(*p_head ==NULL)// 链表为空链表
	{
		*p_head = p_new;
		p_new->next=NULL;
		return ;
	}
	while((p_new->num >= pb->num)  && (pb->next !=NULL) )
	{
		pf=pb;
		pb=pb->next;
	}

	if(p_new->num < pb->num)//找到一个节点的num比新来的节点num大，插在pb的前面
	{
		if(pb== *p_head)//找到的节点是头节点，插在最前面
		{
			p_new->next= *p_head;
			*p_head =p_new;
		}
		else
		{
			pf->next=p_new;
			p_new->next = pb;
		}
	}		
	else//没有找到pb的num比p_new->num大的节点，插在最后
	{
		pb->next =p_new;
		p_new->next =NULL;
	}



}
int main()
{
	STU *head=NULL,*p_new=NULL;
	int num,i;
	printf("请输入链表初始个数:\n");
	scanf("%d",&num);
	for(i=0;i<num;i++)
	{	
		p_new=(STU*)malloc(sizeof(STU));//申请一个新节点
		printf("请输入学号、分数、名字:\n");	//给新节点赋值
		scanf("%d %d %s",&p_new->num,&p_new->score,p_new->name);
		link_creat_head(&head,p_new);	//将新节点加入链表
	}
	link_print(head);
	while(1)
	{
		printf("请输入您要插入的节点的 num score name\n");
		p_new=(STU*)malloc(sizeof(STU));//申请一个新节点
		scanf("%d %d %s",&p_new->num,&p_new->score,p_new->name);
		link_insert_num(&head,p_new);
		link_print(head);
	}
}
