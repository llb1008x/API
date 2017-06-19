#ifndef __link_h__
#define __link_h__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct student
{ 					//数据域
	int num;		//学号
	float score;	//分数
	char name[20];
	struct student *next;//指针域
}STU;

extern void link_creat_end(STU **head,STU *p_new);

extern void link_creat_head(STU **head,STU *p_new);

extern STU* search_name_link(STU *p_mov,char *name);

extern STU* search_num_link(STU *p_mov,int num);

extern void print_link(STU *head);

extern int delet_num_link(STU **head,int num);

extern void insert_num_link(STU **head,STU *p_new);

extern void help();

extern void free_link(STU**head);

extern void select_order_link(STU *head);

extern void reverse_link(STU**head);

#endif