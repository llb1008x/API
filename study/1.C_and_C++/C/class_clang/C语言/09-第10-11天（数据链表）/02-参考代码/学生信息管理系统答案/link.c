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
	if(*head==NULL)		//����Ϊ��
	{
		*head=p_new;
		p_new->next=NULL;
	}
	else				//����Ϊ��
	{
		while(p_mov!=NULL)
		{
			if(p_new->num<p_mov->num)
			{
				if(p_mov==*head)		//���ڵ�һ���ڵ�֮ǰ
				{
					p_new->next=*head;
					*head=p_new;
				}
				else					//�����м�ڵ�
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
		if(flag==0)						//��������
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
		if(num==p_mov->num)				//�ҵ���Ҫɾ���Ľ׶�
		{
			if(p_mov==*head)			//ɾ����һ���ڵ�
				*head=(*head)->next;
			else
				p_before->next = p_mov->next;	//ɾ������Ľڵ�
			flag=1;
			free(p_mov);				//�ͷ�Ҫɾ���Ľڵ�
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
	if(*head==NULL)	//����һ�μ�������Ϊ��ʱ��headִ��p_new
	{
		*head=p_new;
		p_new->next=NULL;
	}
	else	//�ڶ��μ��Ժ��������
	{		
		p_new->next = *head;
		*head=p_new;
	}
}

void link_creat_head(STU **head,STU *p_new)
{
	STU *p_mov=*head;
	if(*head==NULL)	//����һ�μ�������Ϊ��ʱ��headִ��p_new
	{
		*head=p_new;
		p_new->next=NULL;
	}
	else	//�ڶ��μ��Ժ��������
	{		
		while(p_mov->next!=NULL)
		{
			p_mov=p_mov->next;	//�ҵ�ԭ����������һ���ڵ�
		}	
		p_mov->next=p_new;		//��������Ľڵ��������
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
		printf("����Ϊ��\n");
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
	printf("**0.����			**\n");
	printf("**1.����������Ϣ		**\n");
	printf("**2.����������Ϣ		**\n");
	printf("**3.����������			**\n");
	printf("**4.��ѧ�Ų���			**\n");
	printf("**5.ɾ��ָ��ѧ�ŵ��˵���Ϣ	**\n");
	printf("**6.ɾ��ָ���������˵���Ϣ	**\n");
	printf("**7.�����µ�ѧ������Ϣ  	**\n");
	printf("**8.����������			**\n");
	printf("**9.��ѧ������			**\n");
	printf("**10.�˳���Ϣ����ϵͳ		**\n");
	printf("**11.ʵ�����������		**\n");
	//printf("**9.��ѧ������			**\n");
	printf("*********************************\n");
}

