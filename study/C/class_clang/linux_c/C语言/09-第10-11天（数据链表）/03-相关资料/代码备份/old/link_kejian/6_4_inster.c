//=========================================================================
//  工程名称：	LinkInster
//  功能描述： 	链表插入实验
//				写一个函数，在链表中指定位置插入一个结点				           
//  涉及的库：	
//  组成文件：	main.c  stdio.h
//	程序分析：	
//	1. 原表是空表，只需使head指向被插结点即可；
//	2. 被插结点值最小，应插入第一结点之前。这种情况下使head指向被插结点，被插结点的指针域指向原来的第一结点则可；
//	3. 在其它位置插入，这种情况下，使插入位置的前一结点的指针域指向被插结点，使被插结点的指针域指向插入位置的后一结点；
//	4. 在表末插入，这种情况下使原表末结点指针域指向被插结点，被插结点指针域置为NULL；
//	硬件连接：	无		
//	维护记录：	2008-4-18 v1.0		by xdw
//=========================================================================
typedef struct stu
{
	int num;
	int age;
	struct stu *next;
}TYPE;

TYPE *insert(TYPE * head,TYPE *pi)
{
	TYPE *pf,*pb;
	pb=head;
	
	while( (pi->num = pb->num) && (pb->next!=NULL) )	//判断所要插入节点序号
	{
		pf=pb;											//节点后移
		pb=pb->next;
	}							
	if(pi->num <= pb->num)								//找到所要插入节点位置
	{
		if(head==pb)									//在第一结点之前插入
		{
			pi->next=head;								
			head = pi;
		}												
		else
		{	
			pf->next=pi;								//在中间位置插入
			pi->next=pb; 
		}	
	}
	else
	{
		pb->next=pi;									//在表末插入
		pi->next=NULL;
	} 							

	return head;
}