//=========================================================================
//  工程名称：	LinkDelete
//  功能描述： 	链表删除实验
//				写一个函数，删除链表中的指定结点					           
//  涉及的库：	
//  组成文件：	main.c  stdio.h
//	程序分析：	写一个函数，删除链表中的指定结点。删除一个结点有两种情况
//				被删除结点是第一个结点，使head指向第二个结点即可。即head=pb->next
//				被删结点不是第一个结点，使被删结点的前一结点指向被删结点的后一结点即可。
//				即pf->next=pb->next
//	硬件连接：	无
//	维护记录：	2008-4-18 v1.0		by xdw
//=========================================================================
typedef struct stu
{
	int num;
	int age;
	struct stu *next;
}TYPE;


TYPE * delete(TYPE * head,int num)
{
	TYPE *pf,*pb;
	
	if(head == NULL)							//链表为空
	{ 
		printf("\nempty list!\n");
		return NULL;
	}

	pb = head;	
	while( (pb->num!=num) && (pb->next!=NULL) )	//当不是要删除的结点，而且也不是最后一个结点时，继续循环											
	{
		pf=pb;
		pb=pb->next;							//pf指向当前结点，pb指向下一结点
	}
	
	if( pb->num == num )
	{
		if( pb==head )							//如找到被删结点，且为第一结点，则使head指向第二个结点
			head=pb->next;						
		else 
			pf->next=pb->next;					//否则使pf所指结点的指针指向下一结点
			
		free(pb);								//释放删除节点所占空间
		printf("The node is deleted\n");
	}
	else
	{
		printf("The node not been foud!\n");
	}
	return head;
}