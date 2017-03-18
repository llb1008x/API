/*************************************************************************
	> File Name: test_link.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年03月18日 星期六 22时49分40秒

	练习链表的使用
 ************************************************************************/

#include<stdio.h>

#define  NUM   10

/*****************************
 *方案一：多项式的表示和使用
 ****************************/
/*typedef struct PolyNode *Polynomial;
struct PolyNode
{
	int coef;
	int expon;
	Polynomial link;
};
*/

typedef struct LNode *List;
struct LNode{

	ElementType Data[NUM];
	int Last;
};

struct LNode L;
List PtrL;


/*****************************n
 ×建立空的顺序表
 * **************************/
List MakeEmpty
{
	List PtrL;
	PtrL=(List)malloc(sizeof(struct LNode));
	PtrL->Last=-1;
	return PtrL;
}


int main()
{

	return 0;
}
