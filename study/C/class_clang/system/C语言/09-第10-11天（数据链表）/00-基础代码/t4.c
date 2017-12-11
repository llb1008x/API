#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct student{
	int num;
	char name[30];
	struct student *next;
}STU;

void link_print(STU *head)
{
	STU *pb = head;
	
	printf("学号\t姓名\n");
	while(pb != NULL){
		printf("%d\t%s\n",pb->num,pb->name);
		pb = pb->next;
	}
}

STU *link_free(STU *head)
{
	STU *pb = head,*pf;
	
	while(pb != NULL){
		pf = pb;
		pb = pb->next;
		free(pf);
	}
	return NULL;
}

STU *link_create(int num)
{
	int i;
	STU *head,*pb,*pf = NULL;
	
	printf("学号\t姓名\n");
	for(i=0;i<num;i++){
		pb = malloc(sizeof(STU));
		scanf("%d%s",&pb->num,pb->name);
		if(i == 0){
			head = pb;
			pf = pb;
		}else{
			pf->next = pb;
			pf = pb;
		}
	}
	if(pf == NULL)
		return NULL;
	pf->next = NULL;
	return head;
}
#if 0
//倒序创建
STU *link_insert(STU *head,STU *p_new)
{
	if(head == NULL){
		head = p_new;
		head->next = NULL;
	}else{
		p_new->next = head;
		head = p_new;	
	}
	return head;
}
#else
//顺序创建
STU *link_insert(STU *head,STU *p_new)
{
	STU *pf = head;
	if(head == NULL){
		head = p_new;
		head->next = NULL;
	}else{
		while(pf->next != NULL)
			pf = pf->next;
		pf->next = p_new;
		p_new->next = NULL;
	}
	return head;
}
#endif
STU *link_search_by_num(STU *head,int num)
{
	STU *pb = head;
	
	while(pb != NULL){
		if(pb->num == num)
			break;
		pb = pb->next;
	}	
	return pb;
}
STU *link_search_by_name(STU *head,char *name)
{
	STU *pb = head;
	
	while(pb != NULL){
		if(strcmp(pb->name,name) == 0)
			break;
		pb = pb->next;
	}	
	return pb;
}
STU *link_delete_by_num(STU *head,int num)
{
	STU *pb = head,*pf = head;
	
	while(pb != NULL){
		if(pb->num == num){
			if(pb == head)
				head = pb->next;
			pf->next = pb->next;
			free(pb);
			break;
		}
		pf = pb;
		pb = pb->next;
	}	
	return head;
}
STU *link_reverse(STU *head)
{
	STU *pf,*pb,*ps;
	
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
}
STU *link_order(STU *head)
{
	STU *pf = head,*pb = head;
	STU tmp;

	while(pf->next != NULL){
		pb = pf->next;
		while(pb != NULL){
			if(pf->num > pb->num){
				//交换两个节点
				tmp = *pf;
				*pf = *pb;
				*pb = tmp;
				//将指针再换回原来的指向
				tmp.next = pf->next;
				pf->next = pb->next;
				pb->next = tmp.next;
			}
			pb = pb->next;
		}
		pf = pf->next;
	}
	return head;
}

int main()
{
	int i,num;
	char name[30];
	STU *head = NULL,*p_new,*p;//head指向一个链表,即第一个节点
	
	printf("请输入你要创建的节点数量:\n");
	scanf("%d",&num);
	printf("学号\t姓名\n");
	for(i=0;i<num;i++){
		p_new = malloc(sizeof(STU));
		scanf("%d%s",&p_new->num,p_new->name);
		head = link_insert(head,p_new);	
	}
	link_print(head);
#if 0	
	printf("请输入你要查找的姓名:\n");
	scanf("%s",name);
	p = link_search_by_name(head,name);//按学号查找
	if(p != NULL)
		printf("查找到结果:%d\t%s\n",p->num,p->name);
	else
		printf("未找到你要的结果\n");
	printf("请输入你要删除的学生学号:\n");
	scanf("%d",&num);
	head = link_delete_by_num(head,num);//按学号删除
	link_print(head);
#endif	
	printf("\n逆序排列后:\n");
	head = link_reverse(head);//逆序排列
	link_print(head);
	printf("\n排列后:\n");
	head = link_order(head);//逆序排列
	link_print(head);
	
	head = link_free(head);
	return 0;
}
