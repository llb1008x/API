#include "link.h"
void reverse_link(STU**head)
{
	STU *p1,*p2,*temp;
	p1=*head;
	p2=(*head)->next;
	p1->next=NULL;
	while(p2!=NULL)
	{
		temp=p2->next;
		p2->next=p1;
		p1=p2;
		p2=temp;
	}
	*head=p1;
}


void select_order_link(STU *head)
{
	STU *p1,*p2,temp;
	p1=head;
	while(p1->next!=NULL)
	{
		p2=p1->next;
		while(p2!=NULL)
		{
			if(p1->num>p2->num)
			{
				temp=*p1;
				*p1=*p2;
				*p2=temp;
				temp.next = p1->next;
				p1->next=p2->next;
				p2->next=temp.next;
			}
			p2= p2->next;
		}
		p1=p1->next;
	}
}
void insert_num_link(STU **head,STU *p_new)
{
	STU* p_mov=*head,*p_before=*head;
	int flag =0;
	if(*head==NULL)		//链表为空
	{
		*head=p_new;
		p_new->next=NULL;
	}
	else				//链表不为空
	{
		while(p_mov!=NULL)
		{
			if(p_new->num<p_mov->num)
			{
				if(p_mov==*head)		//插在第一个节点之前
				{
					p_new->next=*head;
					*head=p_new;
				}
				else					//插在中间节点
				{
					p_before->next=p_new;
					p_new->next=p_mov;
				}
				flag=1;
				break;
			}
			p_before=p_mov;
			p_mov=p_mov->next;
		}
		if(flag==0)						//在最后插入
		{
			p_before->next=p_new;
			p_new->next=NULL;
		}
	}
}

int delet_num_link(STU **head,int num)
{
	STU* p_mov=*head,*p_before=*head;
	int flag =0;
	while(p_mov!=NULL)
	{
		if(num==p_mov->num)				//找到了要删除的阶段
		{
			if(p_mov==*head)			//删除第一个节点
				*head=(*head)->next;
			else
				p_before->next = p_mov->next;	//删除后面的节点
			flag=1;
			free(p_mov);				//释放要删除的节点
			break;
		}
		p_before=p_mov;
		p_mov=p_mov->next;
	}
	return flag;
}

void link_creat_end(STU **head,STU *p_new)
{
	STU *p_mov=*head;
	if(*head==NULL)	//当第一次加入链表为空时，head执行p_new
	{
		*head=p_new;
		p_new->next=NULL;
	}
	else	//第二次及以后加入链表
	{		
		p_new->next = *head;
		*head=p_new;
	}
}

void link_creat_head(STU **head,STU *p_new)
{
	STU *p_mov=*head;
	if(*head==NULL)	//当第一次加入链表为空时，head执行p_new
	{
		*head=p_new;
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
STU* search_name_link(STU *p_mov,char *name)
{
	while(p_mov!=NULL)
	{
		if(strcmp(name,p_mov->name)==0)
			break;
		p_mov=p_mov->next;
	}
	return p_mov;
}

STU* search_num_link(STU *p_mov,int num)
{
	while(p_mov!=NULL)
	{
		if(num==p_mov->num)
			break;
		p_mov=p_mov->next;
	}
	return p_mov;
}

void free_link(STU**head)
{
	STU *temp;
	while(*head!=NULL)
	{
		temp=(*head)->next;
		free(*head);
		*head=temp;
	}
}

void print_link(STU *head)
{
	STU* p_mov=head;
	if(head==NULL)
		printf("链表为空\n");
	else
	{
		while(p_mov!=NULL)
		{
			printf("%d %f %s\n",p_mov->num,p_mov->score,p_mov->name);
			p_mov=p_mov->next;
		}
	}
}

void help()
{
	printf("*********************************\n");
	printf("**0.帮助			**\n");
	printf("**1.输入宿舍信息		**\n");
	printf("**2.遍历宿舍信息		**\n");
	printf("**3.按姓名查找			**\n");
	printf("**4.按学号查找			**\n");
	printf("**5.删除指定学号的人的信息	**\n");
	printf("**6.删除指定姓名的人的信息	**\n");
	printf("**7.插入新的学生的信息  	**\n");
	printf("**8.按姓名排序			**\n");
	printf("**9.按学号排序			**\n");
	printf("**10.退出信息管理系统		**\n");
	printf("**11.实现链表的逆序		**\n");
	//printf("**9.按学号排序			**\n");
	printf("*********************************\n");
}

