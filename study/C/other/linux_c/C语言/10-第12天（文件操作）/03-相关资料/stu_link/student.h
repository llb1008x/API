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

#define ORDER_BY_NUM	1		//根据学号排序
#define ORDER_BY_SCORE	2		//根据成绩排序

#define FIND_BY_NAME	1		//根据姓名查找
#define FIND_BY_NUM		2		//根据学号查找

/***********************************************
*将node 添加到 *phead 所指的链表末尾
***********************************************/
pTYPE add_node(ppTYPE phead, pTYPE node);

/***********************************************
*从 *phead 所指的链表中删除 node 所指的节点
***********************************************/
pTYPE del_node(ppTYPE phead, pTYPE node);

/***********************************************
*从 遍历 head 所指的链表
***********************************************/
void print_link(pTYPE head);

/***********************************************
* 交换两个节点（算法：只交换两个节点的数据域）
***********************************************/
void change_node(pTYPE src1, pTYPE src2);

/***********************************************
*根据命令cmd 和 参数arg来查找学生信息
* cmd 为 FIND_BY_NAME 则 arg表示 姓名
* cmd 为 FIND_BY_NUM  则 arg表示 学号
***********************************************/
pTYPE find_node(pTYPE head, int cmd, void *arg);

/***********************************************
* 根据cmd 对链表排序
* cmd 为 ORDER_BY_SCORE 则 根据 成绩 排序
* cmd 为 ORDER_BY_NUM   则 根据 学号 排序
***********************************************/
void order_link(pTYPE head,int cmd);

/***********************************************
* 释放整个链表
***********************************************/
void free_link(pTYPE head);

#endif


//add node before pos


