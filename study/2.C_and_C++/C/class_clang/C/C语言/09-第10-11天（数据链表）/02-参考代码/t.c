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
//有序插入
STU *link_insert_order(STU *head,STU *p_new)
{
	STU *pb = head,*pf = head;
	if(head == NULL){
		head = p_new;
		head->next = NULL;
	}else{
		while((p_new->num >= pb->num)&&(pb->next != NULL)){
			pf = pb;
			pb = pb->next;
		}
		if(p_new->num < pb->num){
			if(pb == head){
				head = p_new;
				p_new->next = pb;
			}else{
				pf->next = p_new;
				p_new->next = pb;
			}
		}
		else{//找到结尾
			pb->next = p_new;
			p_new->next = NULL;
		}
	}
	return head;
}
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
void print_help(void)
{
	printf("*********************************\n");
	printf("**0.帮助			**\n");
	printf("**1.创建宿舍信息		**\n");
	printf("**2.遍历宿舍信息		**\n");
	printf("**3.按姓名查找			**\n");
	printf("**4.按学号查找			**\n");
	printf("**5.删除指定学号的人的信息	**\n");
	printf("**6.删除指定姓名的人的信息	**\n");
	printf("**7.插入新的学生的信息  	**\n");
	printf("**8.按升序排序			**\n");
	printf("**9.按降序排序			**\n");
	printf("**10.退出信息管理系统		**\n");
	printf("**11.实现信息的逆序		**\n");
	printf("*********************************\n");
}
int main()
{
	int i,num;
	char name[30];
	STU *head = NULL,*p_new,*p;//head指向一个链表,即第一个节点
	print_help();
	while(1){
		scanf("%d",&num);
		switch(num){
			case 0:
				print_help();
				break;
			case 1:
				printf("请输入你要创建的室友数量:\n");
				scanf("%d",&num);
				printf("学号\t姓名\n");
				for(i=0;i<num;i++){
					p_new = malloc(sizeof(STU));
					scanf("%d%s",&p_new->num,p_new->name);
					head = link_insert_order(head,p_new);	
				}
				break;
			case 2:
				link_print(head);
				break;
			case 3:
				printf("请输入你要查找的姓名:\n");
				scanf("%s",name);
				p = link_search_by_name(head,name);//按学号查找
				if(p != NULL)
					printf("查找到结果:%d\t%s\n",p->num,p->name);
				else
					printf("未找到你要的结果\n");
				break;
			case 4:	
				break;
			case 5:
				printf("请输入你要删除的学生学号:\n");
				scanf("%d",&num);
				head = link_delete_by_num(head,num);//按学号删除	
				break;
			case 6:	
				break;
			case 7:
				printf("请输入你要插入的信息:\n");
				printf("学号\t姓名\n");
				p_new = malloc(sizeof(STU));
				scanf("%d%s",&p_new->num,p_new->name);
				head = link_insert_order(head,p_new);			
				break;
			case 8:
				head = link_order(head);//升序排列
				break;
			case 9:
				head = link_order(head);//升序排列
				head = link_reverse(head);//逆序排列
				break;
			case 10:
				head = link_free(head);
				return 0;
				break;
			case 11:
				head = link_reverse(head);//逆序排列	
				break;
		}
	}
	head = link_free(head);
	return 0;
}
