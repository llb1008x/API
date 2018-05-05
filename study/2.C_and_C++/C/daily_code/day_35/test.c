/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月27日 星期二 19时54分54秒
	中国大学慕课网
	第二周
	
逆序输出（10分）

题目内容：

你的程序会读入一系列的正整数，预先不知道正整数的数量，一旦读到-1，就表示输入结束。
然后，按照和输入相反的顺序输出所读到的数字，不包括最后标识结束的-1。
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//链表数据结构
typedef struct node{

	int num;
	struct node *next;
}NODE;


//插入节点
NODE *link_insert(NODE *head,NODE *p_new)
{
	NODE *pf=head;
	
	if(head==NULL){
	
		head=p_new;
		head->next=NULL;
	}else {
	
		while(pf->next!=NULL){
			pf=pf->next;
		}
		
		pf->next=p_new;
		p_new->next=NULL;
	}

	return head;
}

//遍历链表
void link_print(NODE *head)
{
	
	NODE *pb=head;
	
	printf("数据：\n");
	while(pb!=NULL){
	
		printf("num->%d\n",pb->num);
		pb=pb->next;
	}
}

//链表的反转
NODE *link_reverse(NODE *head)
{
	NODE *pf,*pb,*ps;
	
	ps = pb = pf = head;
	if((head == NULL)||(head->next == NULL))
		return head;
	else if(head->next->next == NULL)
		pb = pf->next;
	pb = pf->next;
	ps = pb->next;
	while(pb->next != NULL){
		pb->next = pf;
		pf = pb;
		pb = ps;
		ps = ps->next;
	}
	pb->next = pf;
	head->next = NULL;
	head = pb;
	return head;
	
	return head;
}


int main()
{
/*
	int number[]={1,2,3,4,5,6,7,8,9,12,-1};
	int i;
	int len=sizeof(number)/sizeof(number[0]);
	printf("len->%d\n",len);
*/
		
	int num,i=0;
	NODE *head=NULL,*p_new=NULL;
	
	printf("请输入数据:\n");
/*
	do{

		p_new=(NODE *)malloc(sizeof(NODE));
		if(p_new==NULL){
		
		    printf("error,not enough space\n");
		    return -1;
		}
		scanf("%d",&p_new->num);
		head=link_insert(head,p_new);
		
	}while(p_new->num!=-1);
*/

    while(1){
        
        p_new=(NODE *)malloc(sizeof(NODE));
		if(p_new==NULL){
		
		    printf("error,not enough space\n");
		    return -1;
		}
		scanf("%d",&p_new->num);
		if(p_new->num==-1){
		
		    printf("termination\n");
		    break;
		}
		
		head=link_insert(head,p_new);
		
    }

	printf("输入结束\n");
	link_print(head);
	
	head=link_reverse(head);
	link_print(head);
	free(head);
	
	return 0;
}
