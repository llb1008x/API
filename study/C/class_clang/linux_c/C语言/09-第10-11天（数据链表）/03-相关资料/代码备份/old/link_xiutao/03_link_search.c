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
int main()
{
	STU * head;
	STU *pb;
	int num;
	head=creat(3);
	//link_print(head);
	printf("请输入您要查询的学生的num\n");
	scanf("%d",&num);
	pb=link_search(head,num);
	if(pb!=NULL)//找到了
		printf("找到了 : %d %d \n",pb->num,pb->age);
	else
		printf("查无此人\n");
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
	return pb;
}


