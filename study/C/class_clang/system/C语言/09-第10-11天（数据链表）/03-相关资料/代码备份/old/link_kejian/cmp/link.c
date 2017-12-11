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
// 语法格式：	creat(int n)
// 实现功能：	创建一个具有n个节点的链表，并对其值进行初始化
// 参数：		n: 链表的长度，即节点的个数
// 返回值：		所创建链表的首地址
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
// 语法格式：	delete(TYPE * head,int num)
// 实现功能：	删除给定序号所指向的节点
// 参数：		*head:待删除链表
//				num： 所需删除的节点
// 返回值：		删除指定节点后的新链表首址
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
// 语法格式：	insert(TYPE * head,TYPE * pi)
// 实现功能：	将新申请的节点加入到指定链表中
// 参数：		*head:待插入链表
//				* pi：带插入节点
// 返回值：		插入指定节点后的新链表首址
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
// 语法格式：	print(TYPE * head)
// 实现功能：	打印指定链表中的全部节点数据
// 参数：		*head：待打印的链表首址
// 返回值：		无
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