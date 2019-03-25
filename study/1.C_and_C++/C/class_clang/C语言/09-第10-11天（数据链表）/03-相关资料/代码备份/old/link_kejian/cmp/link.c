//========================================================================
//  The information contained herein is the exclusive property of
//  sunplusedu. And shall not be distributed, reproduced,
//  or disclosed in whole in part without prior written permission.
//  (C) COPYRIGHT 2003  sunplusedu.
//  ALL RIGHTS RESERVED
//  The entire notice above must be reproduced on all authorized copies.
//=========================================================================
#include <stdio.h>
#include <stdlib.h>
//=============================================================
// �﷨��ʽ��	creat(int n)
// ʵ�ֹ��ܣ�	����һ������n���ڵ������������ֵ���г�ʼ��
// ������		n: ����ĳ��ȣ����ڵ�ĸ���
// ����ֵ��		������������׵�ַ
//=============================================================
TYPE * creat(int n)
{
	struct stu *head,*pf,*pb;
	int i;
	for(i=0;i<n;i++)
	{
		pb=(TYPE *)malloc(LEN);
		printf("input Number and Age:\n");
		scanf("%d%d",&pb->num,&pb->age);
		if(i==0)
			pf=head=pb;
		else 
			pf->next=pb;
			pb->next=NULL;
			pf=pb;
	}
	return(head);
}
//=============================================================
// �﷨��ʽ��	delete(TYPE * head,int num)
// ʵ�ֹ��ܣ�	ɾ�����������ָ��Ľڵ�
// ������		*head:��ɾ������
//				num�� ����ɾ���Ľڵ�
// ����ֵ��		ɾ��ָ���ڵ�����������ַ
//=============================================================
TYPE * delete(TYPE * head,int num)
{
	TYPE *pf,*pb;
	if(head==NULL)
	{ 
		printf("\nempty list!\n");
		goto end;
	}
	pb=head;
	while (pb->num!=num && pb->next!=NULL)
	  {
		pf=pb;
		pb=pb->next;
	  } 
	if(pb->num==num)
	{ 
	  if(pb==head) 
		 head=pb->next;
	  else pf->next=pb->next;
		 printf("The node is deleted\n"); 
	}
	else
	  free(pb);
	  printf("The node not been found!\n");
end:
	return head;
}
//=============================================================
// �﷨��ʽ��	insert(TYPE * head,TYPE * pi)
// ʵ�ֹ��ܣ�	��������Ľڵ���뵽ָ��������
// ������		*head:����������
//				* pi��������ڵ�
// ����ֵ��		����ָ���ڵ�����������ַ
//=============================================================
TYPE * insert(TYPE * head,TYPE * pi)
{
	TYPE *pb ,*pf;
	pb=head;
	
	if(head==NULL)
	  { 
		head=pi;
		pi->next=NULL; 
	  }
	else
	  {
		while((pi->num>pb->num)&&(pb->next!=NULL))
		{ 
			pf=pb;
			pb=pb->next; 
		}
		
	  	if(pi->num<=pb->num)
		{ 
		  if(head==pb) 
			head=pi;
		  else 
			pf->next=pi;
			pi->next=pb; 
		}
		else
		{ 
			pb->next=pi;
			pi->next=NULL;
		}
	}
return head;
}
//=============================================================
// �﷨��ʽ��	print(TYPE * head)
// ʵ�ֹ��ܣ�	��ӡָ�������е�ȫ���ڵ�����
// ������		*head������ӡ��������ַ
// ����ֵ��		��
//=============================================================
void print(TYPE * head)
{
	printf("Number\t\tAge\n");
	while(head!=NULL)
	 {
		printf("%d\t\t%d\n",head->num,head->age);
		head=head->next;
	 }
}