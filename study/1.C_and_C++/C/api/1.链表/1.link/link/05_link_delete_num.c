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
void link_delete_num(STU **p_head,int num)
{
	STU *pb,*pf;
	pb=pf=*p_head;
	if(*p_head ==NULL)//链表为空，不用删
	{
		printf("链表为空，没有您要删的节点");\
		return ;
	}
	while(pb->num != num && pb->next !=NULL)//循环找，要删除的节点
	{
		pf=pb;
		pb=pb->next;
	}
	if(pb->num == num)//找到了一个节点的num和num相同
	{
		if(pb == *p_head)//要删除的节点是头节点
		{
			*p_head = pb->next;
		}
		else
		{
			pf->next = pb->next;
		}
		free(pb);
	}
	else//没有找到
	{
		printf("没有您要删除的节点\n");
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
		printf("请输入您要删除的节点的num\n");
		scanf("%d",&num);
		link_delete_num(&head,num);
		link_print(head);
	}
	return 0;
}
