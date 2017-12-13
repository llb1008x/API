#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "link.h"
#include "student.h"
/***********************************************
*将node 添加到 *phead 所指的链表末尾
***********************************************/
pTYPE add_node(ppTYPE phead, pTYPE node)
{
	*phead =(pTYPE)_add_node((ppLINK)phead, (pLINK)node, NULL);
	return *phead;
}

/***********************************************
*从 *phead 所指的链表中删除 node 所指的节点
***********************************************/
pTYPE del_node(ppTYPE phead, pTYPE node)
{
	*phead =(pTYPE)_del_node((ppLINK)phead, (pLINK)node);
	return *phead;
}

/***********************************************
*从 遍历 head 所指的链表
***********************************************/
void print_link(pTYPE head)
{
	pTYPE pb = head;
	printf("\t%4s\t%3s\t%4s\n", "学号", "姓名", "成绩");
	while(pb!=NULL)
	{
		printf("\t%4d\t%3s\t%4d\n", pb->num, pb->name, pb->score);
		pb=pb->next;
	}
}

/***********************************************
* 交换两个节点（算法：只交换两个节点的数据域）
***********************************************/
void change_node(pTYPE src1, pTYPE src2)
{
	TYPE temp;
	//copyszie 表示要交换的数据大小，即数据域大小
	int copysize = sizeof(TYPE) - 2*sizeof(src1->next);
	//stemp 表示临时交换区的数据域地址
	char *stemp = (char *)&temp+2*sizeof(src1->next);
	//s1 节点1 的数据域地址
	char *s1 = (char *)src1+2*sizeof(src1->next);
	//s2 节点2 的数据域地址
	char *s2 = (char *)src2+2*sizeof(src1->next);
	
	//交换节点1(src1)  和 节点2(src2) 的数据域
	memcpy(stemp, s2, copysize);
	memcpy(s2, s1, copysize);
	memcpy(s1, stemp, copysize);
}

/***********************************************
* 根据cmd 对链表排序
* cmd 为 ORDER_BY_SCORE 则 根据 成绩 排序
* cmd 为 ORDER_BY_NUM   则 根据 学号 排序
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
*根据命令cmd 和 参数arg来查找学生信息
* cmd 为 FIND_BY_NAME 则 arg表示 姓名
* cmd 为 FIND_BY_NUM  则 arg表示 学号
***********************************************/
pTYPE find_node(pTYPE head, int cmd, void *arg)
{
	pTYPE pb = head;
	while(pb!=NULL)
	{
		switch(cmd)
		{
			case FIND_BY_NAME:					//根据姓名查找
				if(strcmp(pb->name, (char *)arg)==0)
					return pb;
				break;
			case FIND_BY_NUM:					//根据学号查找
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
* 释放整个链表
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
