//=========================================================================
//	�ļ�����	1_creat.c
//  ����������	����һ��n�������������ѧ������
//	ά����¼:	2008-4-17 v1.0		by xdw
//=========================================================================
typedef struct stu
{
	int num;
	int age;
	struct stu *next;
}TYPE;

TYPE *creat(int n)							//����һ��ָ�뺯�������ڴ���һ����N���ڵ������
{
	struct stu *head=NULL,*pf=NULL,*pb=NULL;//��������struct stu���͵�ָ�����
	int i=0;								//head����ָ��������׵�ַ��pb����ָ��ոշ���Ŀռ���ַ��pf����ָ���ѽ�������������һ��Ԫ��
	
	for( i=0;i<n;i++ )
	{ 
		pb = (TYPE*)malloc(sizeof(TYPE));	//����һ����̬����ռ�
		printf("input Number and Age\n");
		scanf("%d %d",&pb->num,&pb->age);	//����Ԫ������
	
		if(i==0)							//�ж��Ƿ��һ��ѧ��
			pf=head=pb;						//��������Ŀռ��������
		else 
		{	
			pf->next=pb;	
			pf=pb;
		}				
		
		pb->next=NULL;
	}
	return(head);							//��������������׵�ַ
}
