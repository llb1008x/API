/*************************************************************************
	> File Name: test5.c
	> Author: 
	> Mail: 
	> Created Time: 2018年02月28日 星期三 16时53分43秒
用户输入M,N值，从1至N开始顺序循环数数，每数到M输出该数值，直至全部输出。
写出C程序(约瑟夫环问题)
	
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
//#include "stdafx.h"
#include <iostream>


typedef struct node{
	int data;
	node* next;
}node;

void CreatList(node*& head, node*& tail, int n)
{
	if(n<1){
		head = tail = NULL;
		return;
	}
	
	head = new node();
	head->data = 1;
	head->next = NULL;
	node* p = head;
	
	for(int i=2; i<n+1; i++)
	{
		p->next =new node();
		p = p->next;
		p->data = i;
		p->next = NULL;
	}
	
	tail = p;
	tail -> next = head;
}

 

void Print(node*& head)
{

	node* p = head;
	while(p && p->next != head){
		printf("%d", p->data);
		p = p->next;
	}
	
	if(p){
		printf("%d\n", p->data);

	}
}

void CountPrint(node*& head, node*& tail, int m)
{
	node* pre = tail;
	node* cur = head;
	int cnt = m;
	
	while(cur && cur->next != cur){
		if(cnt != 1){
			cnt--;
			pre = cur;
			cur = cur->next;
		}else{
			printf("%d", cur->data);
			pre->next =cur->next;
			delete cur;
			cur = pre->next;
			cnt = m;
		}
	}
	
	if(cur){
		printf("%d", cur->data);
		delete cur;
		head = tail =NULL;
	}
}

int main()
{
	node* head;
	node* tail;
	int m;
	int n;
	
	scanf("%d", &n);
	scanf("%d", &m);
	CreatList(head, tail, n);
	Print(head);
	CountPrint(head,tail,m);
	
	system("pause");
	
	return 0;
} 
