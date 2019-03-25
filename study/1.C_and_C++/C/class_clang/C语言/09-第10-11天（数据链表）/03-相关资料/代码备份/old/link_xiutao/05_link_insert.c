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
    printf("������Ҫ����ڵ�� num��age\n");
	pi=(STU *)malloc(sizeof(STU));
	scanf("%d %d",&(pi->num),&(pi->age));
	head=link_insert(head,pi);
	printf("����֮�������Ϊ:\n");
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
	if(head==NULL)// �������Ϊ�� ����NULL����Ҫɾ
	{
		printf("link is NULL\n");
		return NULL;
	}
	while((pb->num!=num) && (pb->next!=NULL))//ѭ����pb��numΪnum�Ľڵ�
	{
			pf=pb;//pf��¼һ��pb��λ��
			pb=pb->next;//pbָ����һ���ڵ�
	}
	if(pb->num==num)//�ҵ���Ҫɾ���Ľڵ�
	{
		if(pb==head)//Ҫɾ���Ľڵ���ͷ�ڵ�
		{
			head=head->next;
		}
		else//Ҫɾ���Ľڵ�����ͨ�ڵ�
		{
			pf->next=pb->next;// pf ָ��pb����һ���ڵ�
		}
		free(pb);
	}
	else//û�нڵ��numΪnum
	{
		printf("û����Ҫɾ���Ľڵ�\n");
	}
	return head;
}



STU * link_insert(STU *head,STU *pi)
{
	STU * pf,*pb;
	pb=pf=head;
	if(head==NULL)// �������Ϊ�գ���pi��Ϊhead
	{
		head=pi;
		return head;
	}

	while((pi->num >= pb->num) && (pb->next!=NULL))//ѭ����pb��numΪnum�Ľڵ�
	{
			pf=pb;//pf��¼һ��pb��λ��
			pb=pb->next;//pbָ����һ���ڵ�
	}
	if(pi->num < pb->num)//�ҵ�pb��num��pi��num����,pi ����pb��ǰ��
	{
		if(pb==head)//���ڵ�һ���ڵ��ǰ��
		{
			pi->next=head;//�����Ľڵ�ָ�� ԭ�ȵ�head
			head=pi;//pi ָ��Ľڵ� ���ͷ�ڵ�
		}
		else//������ͨ�ڵ��ǰ��
		{
			pf->next=pi;
			pi->next=pb;
		}
	}
	else//û���ҵ�pb��num��pi��num�󣬲��������β�� 
	{
		pb->next=pi;
		pi->next=NULL;//pi ��Ϊβ�ڵ�
	}
		return head;
}
