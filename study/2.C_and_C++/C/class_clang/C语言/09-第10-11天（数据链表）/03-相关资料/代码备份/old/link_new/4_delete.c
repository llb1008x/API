//=========================================================================
//  �������ƣ�	4_delete.c
//  ���������� 	����ɾ��ʵ��				           
//	���������	дһ��������ɾ�������е�ָ����㡣ɾ��һ��������������
//				��ɾ������ǵ�һ����㣬ʹheadָ��ڶ�����㼴�ɡ���head=pb->next
//				��ɾ��㲻�ǵ�һ����㣬ʹ��ɾ����ǰһ���ָ��ɾ���ĺ�һ��㼴�ɡ�
//				��pf->next=pb->next
//	Ӳ�����ӣ�	��
//	ά����¼��	2008-4-18 v1.0		by xdw
//=========================================================================
typedef struct stu
{
	int num;
	int age;
	struct stu *next;
}TYPE;

TYPE * delete(TYPE * head,int num)
{
	TYPE *pf=NULL,*pb=NULL;
	
	if(head == NULL)							//�������Ϊ��
	{ 
		printf("empty list!\n");
		return NULL;
	}

	pb = head;	
	while( (pb->num!=num) && (pb->next!=NULL) )	//������Ҫɾ���Ľ�㣬����Ҳ�������һ�����ʱ������ѭ��											
	{
		pf=pb;
		pb=pb->next;							//pfָ��ǰ��㣬pbָ����һ���
	}
	
	if( pb->num == num )						//�������while�Ƴ�����Ϊ�ҵ���Ҫɾ���Ľڵ�
	{
		if( pb==head )							//��Ϊ��һ��㣬��ʹheadָ��ڶ������
			head=pb->next;						
		else 
			pf->next=pb->next;					//����ʹpf��ָ����ָ��ָ����һ���
			
		free(pb);								//�ͷ�ɾ���ڵ���ռ�ռ�
		printf("The node is deleted\n");
	}
	else
	{
		printf("The node not been foud!\n");
	}
	return head;
}