//=============================================================
// �﷨��ʽ��	STU * order(STU * head)
// ʵ�ֹ��ܣ�	����ѡ�񷨣���ָ��������Ž�������ֻ����������
// ������		head�������������ַ
// ����ֵ��		�ź��������headָ��
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
				//�Խṹ�������еĳ�Ա���н���
				temp = *pb;	
				*pb = *pf;
				*pf = temp;

				//Ϊ�˱����޸�֮ǰ���������ԣ���Ҫ��ָ�����ٸ�����ȥ
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
