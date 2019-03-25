#include <stdio.h>
#include <stdlib.h>

typedef struct stu
{    
	int num;
	char name[10];
	int telphone;
	struct stu *next;
}TYPE;

extern TYPE *creat(int n);
extern TYPE search(head,name);
extern void point_all(head);
int main(void)
{
	int num;
	char name[10];
	TYPE *head;

	printf("请输入你们宿舍的人数：");
	scanf("%d",&num);

	head = creat(num);

	printf("你所输入的数据为：\n");
	point_all(head);

	printf("请输入你要查找的姓名");

	while(1)
	{
		scanf("%s",name);
		search(head,name);
	}

}
 void point_all(TYPE *head)
 {
	TYPE *temp = head;
	while( temp != NULL)
	{
		printf("%d %s %d\n",temp->num,temp->name,temp->telphone);
		temp = temp->next; 
	}

 }
TYPE search( TYPE *head,char *name)
{
	TYPE *temp = head;
	while(  temp !=NULL )
	{
		if ( strcmp(temp->name,name)==0)
			printf("%d %s %d\n",temp->num,temp->name,temp->telphone);
		temp = temp->next;
	}
	
}



TYPE *creat(int n)						//创建一个指针函数，用于创建一个有N个节点的链表
{
	TYPE *head,*pf,*pb;			//定义三个struct stu类型的指针变量
	int i;								//head用于指向本链表的首地址，pb用于指向刚刚分配的空间首址，pf用于指向已建立的链表的最后一个元素
	
	for(i=0;i<n;i++)
	{ 
		pb=(TYPE*) malloc(sizeof(TYPE));			//申请一个动态链表空间
		printf("input Number name telphone\n");
		scanf("%d %s %d",&pb->num,pb->name,&pb->telphone);//输入元素内容
	
		if(i==0)						//判断是否第一个学生
		pf=head=pb;						//将新申请的空间加入链表
		else 
		{	
			pf->next=pb;				
			pb->next=NULL;
			pf=pb;
	  }
	}
	return(head);						//返回整个链表的首地址
}
