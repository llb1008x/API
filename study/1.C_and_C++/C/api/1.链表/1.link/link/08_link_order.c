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
void link_order(STU *head)
{
	STU *pb,*pf,temp;
	pf=head;
	if(head==NULL)
	{
		printf("链表为空,不用排序\n");
		return ;
	}
	if(head->next ==NULL)
	{
		printf("只有一个节点，不用排序\n");
		return ;
	}

	while(pf->next !=NULL)//以pf指向的节点为基准节点，
	{
		pb=pf->next;//pb从基准元素的下个元素开始
		while(pb!=NULL)
		{
			if(pf->num > pb->num)
			{
				temp=*pb;
				*pb=*pf;
				*pf=temp;

				temp.next=pb->next;
				pb->next=pf->next;
				pf->next=temp.next;
			}
			pb=pb->next;
		}
		pf=pf->next;
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

	printf("\n\n\n\n\n");
	link_order(head);
	
	
	link_print(head);
}
