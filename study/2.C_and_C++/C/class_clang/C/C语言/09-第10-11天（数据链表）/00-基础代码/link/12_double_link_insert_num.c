#include <stdio.h>
#include <stdlib.h>
typedef struct student
{ 					//数据域
	int num;		//学号
	int score;	//分数
	char name[20];
	struct student *front;
	struct student *next;//指针域
}STU;

void double_link_creat_head(STU **p_head,STU *p_new)
{
	STU *p_mov=*p_head;
	if(*p_head==NULL)				//当第一次加入链表为空时，head执行p_new
	{
		*p_head=p_new;
		p_new->front=NULL;
		p_new->next=NULL;
	}
	else	//第二次及以后加入链表
	{		
		while(p_mov->next!=NULL)
		{
			p_mov=p_mov->next;	//找到原有链表的最后一个节点
		}	
		p_mov->next=p_new;		//将新申请的节点加入链表
		p_new->front=p_mov;
		p_new->next=NULL;
	}
}


void double_link_print(STU *head)
{
	STU *pb;
	pb=head;
	if(head==NULL)
	{
		printf("链表为空\n");
		return ;
	}
	while(pb->next!=NULL)
	{
		printf("num=%d score=%d name:%s\n",pb->num,pb->score,pb->name);
		pb=pb->next;
	}
	printf("num=%d score=%d name:%s\n",pb->num,pb->score,pb->name);

	printf("\n\n\n\n");
	while(pb!=NULL)
	{
		printf("num=%d score=%d name:%s\n",pb->num,pb->score,pb->name);
		pb=pb->front;
	}


}
void double_link_insert_num(STU **p_head,STU *p_new)
{
	STU *pb,*pf;
	pb=*p_head;
	if(*p_head == NULL)//链表为空，新来的节点就是头节点
	{
		*p_head=p_new;
		p_new->front=NULL;
		p_new->next=NULL;
		return ;
	}
	while((p_new->num >= pb->num) && (pb->next!=NULL) )
	{
		pb=pb->next ;
	}
	if(p_new->num < pb->num)//找到了一个pb的num比新来的节点的num大，插在pb前边
	{
		if(pb==*p_head)//找到的节点是头节点，插在头节点的前边
		{
			p_new->next=*p_head;
			(*p_head)->front=p_new;
			p_new->front=NULL;
			*p_head=p_new;
		}
		else
		{
			pf=pb->front;//pf指向 找到节点的前一个节点
			
			p_new->next=pb;
			p_new->front=pf;
			pf->next=p_new;
			pb->front=p_new;
		}
	}
	else//所有pb指向节点的num都比p_new指向的节点的num小，插在最后
	{
		pb->next=p_new;
		p_new->front=pb;
		p_new->next=NULL;
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
		double_link_creat_head(&head,p_new);	//将新节点加入链表
	}
	double_link_print(head);
	while(1)
	{
		p_new=(STU*)malloc(sizeof(STU));//申请一个新节点
		printf("请输入您要插入的节点的num score name\n");
		scanf("%d %d %s",&p_new->num,&p_new->score,p_new->name);
		double_link_insert_num(&head,p_new);
		double_link_print(head);
		
	}
}
