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

	printf("���������������������");
	scanf("%d",&num);

	head = creat(num);

	printf("�������������Ϊ��\n");
	point_all(head);

	printf("��������Ҫ���ҵ�����");

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



TYPE *creat(int n)						//����һ��ָ�뺯�������ڴ���һ����N���ڵ������
{
	TYPE *head,*pf,*pb;			//��������struct stu���͵�ָ�����
	int i;								//head����ָ��������׵�ַ��pb����ָ��ոշ���Ŀռ���ַ��pf����ָ���ѽ�������������һ��Ԫ��
	
	for(i=0;i<n;i++)
	{ 
		pb=(TYPE*) malloc(sizeof(TYPE));			//����һ����̬����ռ�
		printf("input Number name telphone\n");
		scanf("%d %s %d",&pb->num,pb->name,&pb->telphone);//����Ԫ������
	
		if(i==0)						//�ж��Ƿ��һ��ѧ��
		pf=head=pb;						//��������Ŀռ��������
		else 
		{	
			pf->next=pb;				
			pb->next=NULL;
			pf=pb;
	  }
	}
	return(head);						//��������������׵�ַ
}
