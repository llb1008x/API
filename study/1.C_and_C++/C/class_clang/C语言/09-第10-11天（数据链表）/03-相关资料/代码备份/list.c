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

	/* @������������㷽ʽ��������
	 * �ص㣺�����õ��������򣬽��½��ҽӵ������β��������
	 * �ɹ���ͨ�����������ӡ�����鿴����
	 */
	for(i=0; i<sizeof(studentdent)/sizeof(studentdent[0]); i++)
		head_disorder = list_add(head_disorder,&studentdent[i]);
	//��ӡ����
	list_print(head_disorder);
	printf("--------------\n");


	/* @����������������㷽ʽ��������
	 * �ص㣺�����õ�����������ģ��½����ҵ������Լ���λ�ý���
	 * ����������������ɹ���ͨ�����������ӡ�����鿴����
	 */
	for(i=0; i<sizeof(studentdent)/sizeof(studentdent[0]); i++)
		head = list_insert(head,&studentdent[i]);
	//��ӡ����
	list_print(head);
	printf("--------------\n");


	/* @�����������һ��������
	 * �ص㣺�����õ�����������ģ��½����ҵ������Լ���λ�ý���
	 * ����������������ɹ���ͨ�����������ӡ�����鿴����
	 */
	head = list_insert(head, &one_stu);
	list_print(head);
	printf("--------------\n");
	

	/* @����������
	 * �ص㣺ͨ�����Ҳ�����������������Ľ�㣬���ҳɹ��������ҽ���ָ�룬
	 * ����ʧ�ܷ��ؿ�ָ��
	 */
	node = list_search(head, 155);
	if(node == NULL)
		printf("search failed!\n");
	else
		printf("name: %s, height: %d, weight %d\n",
				node->name, node->height, node->weight);


	/* @�ͷ�����
	 * �ص㣺�ͷŲ���malloc��ʽ�ֶ������������Ŀռ�
	 * ע�⣺���ڱ��������ǲ��ܽ��������ͷţ�������Ϊʲô��
	 */
//	list_free(head);
	return 0;
}


/* ������Ӻ���
 * ������headΪ����ͷ��nodeΪ����ӽ��
 * ����ֵ����ӳɹ����������head��ͷָ�룩
 */
struct student *list_add(struct student *head, struct student *node)
{
	struct student *mov = head;

	if(head == NULL){	//�������Ϊ�գ�headָ��node����
		head = node;
		node->next = NULL;
		return head;
	}
	//�������ǿգ��ҵ�����β��㣬�ҽ�node����
	while(mov->next != NULL)
		mov = mov->next;
	
	mov->next = node;
	node->next = NULL;
	return head;	
}

/* ����������뺯��
 * ������headΪ����ͷ��nodeΪ��������
 * ����ֵ����ӳɹ����������head��ͷָ�룩
 */
struct student *list_insert(struct student *head, struct student *node)
{
	struct student *mov = head;
	struct student *last = head;

	if(head == NULL){	//�������Ϊ�գ�headָ��node����
		head = node;
		node->next = NULL;
		return head;
	}
	//�������Ϊ��
	while(mov)
	{
		if(mov->height > node->height)
			break;
		last = mov;
		mov = mov->next;
	}

	if(mov == head){	//�������λ��Ϊ�׽ڵ��ǰ��
		head = node;
		node->next = mov;		
	}else if(mov == NULL){	//�������λ��Ϊβ���ĺ���
		last->next = node;
		node->next = NULL;
	}else{					//�������λ��Ϊ�м�λ��
		last->next = node;
		node->next = mov;
	}
	return head;
}

/* �����ͷź���
 * ������headΪ����ͷ
 * ����ֵ����
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

/* ���������㲢��ӡ����
 * ������headΪ����ͷ
 * ����ֵ����
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

/* ������Һ���
 * ������headΪ����ͷ��hgtΪƥ�����
 * ����ֵ�����ҳɹ��Ľ���ַ
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
