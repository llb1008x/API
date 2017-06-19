#ifndef __linker_h__
#define __linker_h__


#define NULL 0
#define LEN sizeof (struct stu)
//=========================================================================
//	文件名：	main.c
//  功能描述：	完成链表建立，结点删除，结点插入等操作的综合练习
//	维护记录:	2008-4-18 v1.0		by xdw
//=========================================================================
typedef struct stu
{
	int num;
	int age;
	struct stu *next;
}TYPE;

extern TYPE * creat(int n);

extern TYPE * delete(TYPE * head,int num);

extern TYPE * insert(TYPE * head,TYPE * pi);

extern void print(TYPE * head);

#endif