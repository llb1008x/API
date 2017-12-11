//========================================================================
//  The information contained herein is the exclusive property of
//  sunplusedu. And shall not be distributed, reproduced,
//  or disclosed in whole in part without prior written permission.
//  (C) COPYRIGHT 2003  sunplusedu.
//  ALL RIGHTS RESERVED
//  The entire notice above must be reproduced on all authorized copies.
//=========================================================================

//=========================================================================
//  工程名称：	StructStudentMolloc
//  功能描述： 	结构体内存动态分配实验
//				申请一个动态空间存储一个学生的信息，并打印出来			           
//  涉及的库：	
//  组成文件：	main.c  stdio.h
//	程序分析：	
//	硬件连接：	无		
//	维护记录：	2008-4-17 v1.0		by xdw
//=========================================================================

#include <stdio.h>
#include "stdlib.h"
//=========================================================================
//	文件名：	main.c
//  功能描述：	申请一个动态空间存储一个学生的信息，并打印出来
//	维护记录:	2008-4-17 v1.0		by xdw
//=========================================================================
struct stu
{
  	int num;
	char *name;
	char sex;
	float score;
}*ps;

int main(void)
{
	ps=(struct stu*)malloc(sizeof(struct stu));	//在系统中申请一个stu类型的存储空间，并把其首址付给ps
	
	ps->num=102;   //以ps为指向结构的指针变量对各成员赋值
	ps->name="Zhang ping";
	ps->sex='M';
	ps->score=62.5;
	
	printf("Number=%d\nName=%s\n",ps->num,ps->name);//打印结构体中的内容
	printf("Sex=%c\nScore=%f\n",ps->sex,ps->score);
	
	free(ps);
}












