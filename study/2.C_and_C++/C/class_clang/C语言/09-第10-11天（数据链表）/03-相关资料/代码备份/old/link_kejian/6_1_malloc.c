//========================================================================
//  The information contained herein is the exclusive property of
//  sunplusedu. And shall not be distributed, reproduced,
//  or disclosed in whole in part without prior written permission.
//  (C) COPYRIGHT 2003  sunplusedu.
//  ALL RIGHTS RESERVED
//  The entire notice above must be reproduced on all authorized copies.
//=========================================================================

//=========================================================================
//  �������ƣ�	StructStudentMolloc
//  ���������� 	�ṹ���ڴ涯̬����ʵ��
//				����һ����̬�ռ�洢һ��ѧ������Ϣ������ӡ����			           
//  �漰�Ŀ⣺	
//  ����ļ���	main.c  stdio.h
//	���������	
//	Ӳ�����ӣ�	��		
//	ά����¼��	2008-4-17 v1.0		by xdw
//=========================================================================

#include <stdio.h>
#include "stdlib.h"
//=========================================================================
//	�ļ�����	main.c
//  ����������	����һ����̬�ռ�洢һ��ѧ������Ϣ������ӡ����
//	ά����¼:	2008-4-17 v1.0		by xdw
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
	ps=(struct stu*)malloc(sizeof(struct stu));	//��ϵͳ������һ��stu���͵Ĵ洢�ռ䣬��������ַ����ps
	
	ps->num=102;   //��psΪָ��ṹ��ָ������Ը���Ա��ֵ
	ps->name="Zhang ping";
	ps->sex='M';
	ps->score=62.5;
	
	printf("Number=%d\nName=%s\n",ps->num,ps->name);//��ӡ�ṹ���е�����
	printf("Sex=%c\nScore=%f\n",ps->sex,ps->score);
	
	free(ps);
}












