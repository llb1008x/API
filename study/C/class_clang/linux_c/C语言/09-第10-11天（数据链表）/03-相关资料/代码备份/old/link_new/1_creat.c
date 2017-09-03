//=========================================================================
//	文件名：	1_creat.c
//  功能描述：	建立一个n个结点的链表，存放学生数据
//	维护记录:	2008-4-17 v1.0		by xdw
//=========================================================================
typedef struct stu
{
	int num;
	int age;
	struct stu *next;
}TYPE;

TYPE *creat(int n)							//创建一个指针函数，用于创建一个有N个节点的链表
{
	struct stu *head=NULL,*pf=NULL,*pb=NULL;//定义三个struct stu类型的指针变量
	int i=0;								//head用于指向本链表的首地址，pb用于指向刚刚分配的空间首址，pf用于指向已建立的链表的最后一个元素
	
	for( i=0;i<n;i++ )
	{ 
		pb = (TYPE*)malloc(sizeof(TYPE));	//申请一个动态链表空间
		printf("input Number and Age\n");
		scanf("%d %d",&pb->num,&pb->age);	//输入元素内容
	
		if(i==0)							//判断是否第一个学生
			pf=head=pb;						//将新申请的空间加入链表
		else 
		{	
			pf->next=pb;	
			pf=pb;
		}				
		
		pb->next=NULL;
	}
	return(head);							//返回整个链表的首地址
}
