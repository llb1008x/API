#include <stdio.h>
#include <stdlib.h>

struct student{
	char name[20];
	int height;
	int weight;
	char sex;
	struct student *next;
};

extern struct student *list_add(struct student *head, struct student *node);
extern struct student *list_insert(struct student *head, struct student *node);
extern void list_free(struct student *head);
extern void list_print(struct student *head);
extern struct student *list_search(struct student *head, int hgt);


struct student studentdent[] = {
	{"yabin",175,65},
	{"liujie",165,50},
	{"haoyu",170,70},
	{"xiaowu",160,65},
	{"xiening",180,60},
	{"hanbiao",185,55},
};
struct student one_stu = {"xiangshuai",200,80};

int main(void)
{
	struct student *head_disorder = NULL;
	struct student *head = NULL;
	struct student *node;
	int i;

	/* @采用添加链表结点方式创建链表
	 * 特点：创建好的链表无序，将新结点挂接到链表的尾部，创建
	 * 成功后通过链表遍历打印函数查看内容
	 */
	for(i=0; i<sizeof(studentdent)/sizeof(studentdent[0]); i++)
		head_disorder = list_add(head_disorder,&studentdent[i]);
	//打印链表
	list_print(head_disorder);
	printf("--------------\n");


	/* @采用有序插入链表结点方式创建链表
	 * 特点：创建好的链表是有序的，新结点会找到合适自己的位置将自
	 * 己插入进链表，创建成功后通过链表遍历打印函数查看内容
	 */
	for(i=0; i<sizeof(studentdent)/sizeof(studentdent[0]); i++)
		head = list_insert(head,&studentdent[i]);
	//打印链表
	list_print(head);
	printf("--------------\n");


	/* @单独有序插入一个链表结点
	 * 特点：创建好的链表是有序的，新结点会找到合适自己的位置将自
	 * 己插入进链表，创建成功后通过链表遍历打印函数查看内容
	 */
	head = list_insert(head, &one_stu);
	list_print(head);
	printf("--------------\n");
	

	/* @查找链表结点
	 * 特点：通过查找参数查找链表中相符的结点，查找成功返回所找结点的指针，
	 * 查找失败返回空指针
	 */
	node = list_search(head, 155);
	if(node == NULL)
		printf("search failed!\n");
	else
		printf("name: %s, height: %d, weight %d\n",
				node->name, node->height, node->weight);


	/* @释放链表
	 * 特点：释放采用malloc方式手动申请的链表结点的空间
	 * 注意：对于本程序，我们不能进行链表释放，请想想为什么？
	 */
//	list_free(head);
	return 0;
}


/* 链表添加函数
 * 参数：head为链表头；node为待添加结点
 * 返回值：添加成功后新链表的head（头指针）
 */
struct student *list_add(struct student *head, struct student *node)
{
	struct student *mov = head;

	if(head == NULL){	//如果链表为空，head指向node即可
		head = node;
		node->next = NULL;
		return head;
	}
	//如果链表非空，找到链表尾结点，挂接node即可
	while(mov->next != NULL)
		mov = mov->next;
	
	mov->next = node;
	node->next = NULL;
	return head;	
}

/* 链表有序插入函数
 * 参数：head为链表头；node为待插入结点
 * 返回值：添加成功后新链表的head（头指针）
 */
struct student *list_insert(struct student *head, struct student *node)
{
	struct student *mov = head;
	struct student *last = head;

	if(head == NULL){	//如果链表为空，head指向node即可
		head = node;
		node->next = NULL;
		return head;
	}
	//如果链表不为空
	while(mov)
	{
		if(mov->height > node->height)
			break;
		last = mov;
		mov = mov->next;
	}

	if(mov == head){	//如果插入位置为首节点的前面
		head = node;
		node->next = mov;		
	}else if(mov == NULL){	//如果插入位置为尾结点的后面
		last->next = node;
		node->next = NULL;
	}else{					//如果插入位置为中间位置
		last->next = node;
		node->next = mov;
	}
	return head;
}

/* 链表释放函数
 * 参数：head为链表头
 * 返回值：无
 */
void list_free(struct student *head)
{
	struct student *mov = head;

	while(mov)
	{
		head = head->next;
		free(mov);
		mov = head;
	}
	return;
}

/* 链表遍历结点并打印函数
 * 参数：head为链表头
 * 返回值：无
 */
void list_print(struct student *head)
{
	struct student *mov = head;
	
	while(mov)
	{
		printf("node message: %s %d %d\n",
			mov->name,mov->height,mov->weight);
		mov = mov->next;
	}
	return;
}

/* 链表查找函数
 * 参数：head为链表头；hgt为匹配参数
 * 返回值：查找成功的结点地址
 */
struct student *list_search(struct student *head, int hgt)
{
	struct student *mov = head;
	while(mov)
	{
		if(mov->height == hgt)
			return mov;
		mov = mov->next;
	}

	return NULL;
}


struct student *list_del(struct student *head,
						 int num)
{
	struct student *mov = head;
	struct student *last = head;

	while(mov)
	{
		if(mov->num == num)
			break;
		last = mov;
		mov = mov->next;
	}

	if(mov == NULL)
		return head;
	else if(mov == head)
		head = mov->next;
	else
		last->next = mov->next;

	free(mov);
	return head;
}
