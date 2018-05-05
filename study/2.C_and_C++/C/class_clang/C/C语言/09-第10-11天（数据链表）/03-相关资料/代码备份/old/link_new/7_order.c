//=============================================================
// 语法格式：	STU * order(STU * head)
// 实现功能：	采用选择法，对指定链表按序号进行排序（只交换数据域）
// 参数：		head：待排序的链首址
// 返回值：		排好序的链表head指针
//=============================================================
 STU * order(STU * head)
{
	STU temp;
	STU *pf=NULL,*pb=NULL;
	pb = pf = head;
	if(head == NULL)		
	{
		printf("list is null\n");
		return NULL;
	}
	while(pf->next!= NULL)
	{	
		pb	= pf->next;
		while(pb!= NULL)
		{
			if(pf->number > pb->number)
			{
				//对结构体中所有的成员进行交换
				temp = *pb;	
				*pb = *pf;
				*pf = temp;

				//为了保持修改之前链表连续性，需要把指针域再给换回去
				temp.next = pb->next;
				pb->next = pf->next;
				pf->next = temp.next;
			}
			pb = pb ->next;
		}
		pf = pf->next;
	}
	return (head);
}
