#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "link.h"
#include "student.h"
/***********************************************
*��node ��ӵ� *phead ��ָ������ĩβ
***********************************************/
pTYPE add_node(ppTYPE phead, pTYPE node)
{
	*phead =(pTYPE)_add_node((ppLINK)phead, (pLINK)node, NULL);
	return *phead;
}

/***********************************************
*�� *phead ��ָ��������ɾ�� node ��ָ�Ľڵ�
***********************************************/
pTYPE del_node(ppTYPE phead, pTYPE node)
{
	*phead =(pTYPE)_del_node((ppLINK)phead, (pLINK)node);
	return *phead;
}

/***********************************************
*�� ���� head ��ָ������
***********************************************/
void print_link(pTYPE head)
{
	pTYPE pb = head;
	printf("\t%4s\t%3s\t%4s\n", "ѧ��", "����", "�ɼ�");
	while(pb!=NULL)
	{
		printf("\t%4d\t%3s\t%4d\n", pb->num, pb->name, pb->score);
		pb=pb->next;
	}
}

/***********************************************
* ���������ڵ㣨�㷨��ֻ���������ڵ��������
***********************************************/
void change_node(pTYPE src1, pTYPE src2)
{
	TYPE temp;
	//copyszie ��ʾҪ���������ݴ�С�����������С
	int copysize = sizeof(TYPE) - 2*sizeof(src1->next);
	//stemp ��ʾ��ʱ���������������ַ
	char *stemp = (char *)&temp+2*sizeof(src1->next);
	//s1 �ڵ�1 ���������ַ
	char *s1 = (char *)src1+2*sizeof(src1->next);
	//s2 �ڵ�2 ���������ַ
	char *s2 = (char *)src2+2*sizeof(src1->next);
	
	//�����ڵ�1(src1)  �� �ڵ�2(src2) ��������
	memcpy(stemp, s2, copysize);
	memcpy(s2, s1, copysize);
	memcpy(s1, stemp, copysize);
}

/***********************************************
* ����cmd ����������
* cmd Ϊ ORDER_BY_SCORE �� ���� �ɼ� ����
* cmd Ϊ ORDER_BY_NUM   �� ���� ѧ�� ����
***********************************************/
void order_link(pTYPE head, int cmd)
{
	pTYPE pi = head;
	pTYPE pj = head;
	while (pi!=NULL)
	{
		pj = pi->next;
		while (pj!=NULL)
		{
			int ti = 0, tj = 0;
			if(ORDER_BY_NUM==cmd)
			{
				ti = pi->num;
				tj = pj->num;
			}
			else if(ORDER_BY_SCORE==cmd)
			{
				ti = pi->score;
				tj = pj->score;
			}
			if(tj < ti)
			{
				change_node(pi, pj);				
			}
			pj = pj->next;
		}
		pi = pi->next;
	}
}

/***********************************************
*��������cmd �� ����arg������ѧ����Ϣ
* cmd Ϊ FIND_BY_NAME �� arg��ʾ ����
* cmd Ϊ FIND_BY_NUM  �� arg��ʾ ѧ��
***********************************************/
pTYPE find_node(pTYPE head, int cmd, void *arg)
{
	pTYPE pb = head;
	while(pb!=NULL)
	{
		switch(cmd)
		{
			case FIND_BY_NAME:					//������������
				if(strcmp(pb->name, (char *)arg)==0)
					return pb;
				break;
			case FIND_BY_NUM:					//����ѧ�Ų���
				if(pb->num == (int)arg)
					return pb;
				break;
			default:
				break;
		}
		pb = pb->next;
	}
	return NULL;
}

/***********************************************
* �ͷ���������
***********************************************/
void free_link(pTYPE head)
{
	pTYPE p = head;
	pTYPE pb = head;
	while(p!=NULL)
	{
		pb = p->next;
		free(p);
		p = pb;
	}
}
