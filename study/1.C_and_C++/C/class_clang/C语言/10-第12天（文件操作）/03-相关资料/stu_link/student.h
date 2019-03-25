#ifndef _STUDENT_H_
#define _STUDENT_H_

typedef struct stu_info
{
	struct stu_info *next;
	struct stu_info *prev;
	
	int num;
	char name[20];
	int score;
}STU, *pSTU, **ppSTU;

typedef STU	TYPE;
typedef pSTU	pTYPE;
typedef ppSTU	ppTYPE;

#define ORDER_BY_NUM	1		//����ѧ������
#define ORDER_BY_SCORE	2		//���ݳɼ�����

#define FIND_BY_NAME	1		//������������
#define FIND_BY_NUM		2		//����ѧ�Ų���

/***********************************************
*��node ��ӵ� *phead ��ָ������ĩβ
***********************************************/
pTYPE add_node(ppTYPE phead, pTYPE node);

/***********************************************
*�� *phead ��ָ��������ɾ�� node ��ָ�Ľڵ�
***********************************************/
pTYPE del_node(ppTYPE phead, pTYPE node);

/***********************************************
*�� ���� head ��ָ������
***********************************************/
void print_link(pTYPE head);

/***********************************************
* ���������ڵ㣨�㷨��ֻ���������ڵ��������
***********************************************/
void change_node(pTYPE src1, pTYPE src2);

/***********************************************
*��������cmd �� ����arg������ѧ����Ϣ
* cmd Ϊ FIND_BY_NAME �� arg��ʾ ����
* cmd Ϊ FIND_BY_NUM  �� arg��ʾ ѧ��
***********************************************/
pTYPE find_node(pTYPE head, int cmd, void *arg);

/***********************************************
* ����cmd ����������
* cmd Ϊ ORDER_BY_SCORE �� ���� �ɼ� ����
* cmd Ϊ ORDER_BY_NUM   �� ���� ѧ�� ����
***********************************************/
void order_link(pTYPE head,int cmd);

/***********************************************
* �ͷ���������
***********************************************/
void free_link(pTYPE head);

#endif


//add node before pos


