#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

#define DATAFILE	"stu.txt"

//help������, �ں�߶���
char *help;

// �������
void add_fun(ppTYPE phead, char *cmd)
{	
	pTYPE pb = (pTYPE)malloc(sizeof(TYPE));
	printf("������Ҫ��ӵ�ѧ����Ϣ\n(ѧ�� ���� �ɼ�)\n");
	scanf("%d %s %d", &pb->num, pb->name, &pb->score);
	pb->next = NULL;
	pb->prev = NULL;
	getchar();
	//��pb��ָ��ѧ����Ϣ��ӵ�������
	add_node(phead, pb);			
}

// ɾ������
void del_fun(ppTYPE phead, char *cmd)
{
	int temp = 0;
	//pָ��del��Ĳ�������������ж� ��"neme" ���� "num"
	//"neme" �� "num" �ֱ��ʾ���� ���� ���Ǹ��� ѧ�� ɾ��
	char *p = cmd+4;
	pTYPE pb = NULL;	
	if (strncmp(p, "name", 4)==0)	//��������ɾ��
	{
		char name[20]="";
		printf("����������:");
		scanf("%s", name);
		getchar();
		pb = find_node(*phead, FIND_BY_NAME, (void*)name);	//�ҵ�����Ϊname��ѧ����Ϣ
	}
	else if(strncmp(p, "num", 3)==0) //����ѧ��ɾ��
	{
		int num = -1;
		printf("������ѧ��:");
		scanf("%d", &num);
		getchar();
		pb = find_node(*phead, FIND_BY_NUM, (void*)num);	//�ҵ�����Ϊname��ѧ����Ϣ
	}
	if (pb!=NULL)
	{
		*phead = del_node(phead, pb);	//ɾ���ϱ��ҵ���ѧ��
	}
	else
	{
		printf("���޴���!\n");
	}
}

// ��������
void find_fun(ppTYPE phead, char *cmd)
{
	char *p = cmd+5;
	//pָ��find��Ĳ�������������ж� ��"num" ���� "name"
	//"num" �� "name" �ֱ��ʾ���� ѧ�� ���Ǹ��� ���� ������
	pTYPE pb = NULL;
	pTYPE ret = NULL;
	
	if (strncmp(p, "name", 4)==0)			//���� ���� 
	{
		char name[20]="";
		printf("������Ҫ���ҵ�ѧ������:");
		scanf("%s", name);
		getchar();
		ret = find_node(*phead, FIND_BY_NAME, name);	//���ҽڵ�
	} 
	else if(strncmp(p, "num", 3)==0)	    //���� ѧ�� 
	{
		int num = 0;
		printf("������Ҫ���ҵ�ѧ��ѧ��:");
		scanf("%d", &num);
		getchar();
		ret = find_node(*phead, FIND_BY_NUM, p);        //���ҽڵ�
	}
	if (ret !=NULL)
	{	//��ӡ�ҵ���ѧ����Ϣ
		printf("\t%4d\t%3s\t%4d\n", ret->num, ret->name, ret->score);
	}
	else
	{
		printf("���޴���!\n");
	}
}

// ��ӡ����
void print_fun(ppTYPE phead, char *cmd)
{
	// ������������ӡ
	print_link(*phead);
}

// ��������
void order_fun(ppTYPE phead, char *cmd)
{
	char *p = cmd+6;
	//pָ��order��Ĳ�������������ж� ��"num" ���� "score"
	//"num" �� "score" �ֱ��ʾ���� ѧ�� ���Ǹ��� �ɼ� ������
	pTYPE pb = NULL;
	if (strncmp(p, "score", 4)==0)		 	 //���� �ɼ�
	{
		order_link(*phead, ORDER_BY_SCORE);	 // ���� 
	}
	else if(strncmp(p, "num", 3)==0)		 //���� ѧ��
	{
		order_link(*phead, ORDER_BY_NUM);	 // ���� 
	}
}


// ��������
void help_fun(ppTYPE phead, char *cmd)
{
	//��ӡ�����˵�
	printf("%s", help);	
}

void save_file(char *file, pTYPE head)
{
	FILE *fp = NULL;
	pTYPE pb = head;
	
	if ((fp=fopen(file, "w+"))==0)
	{
		perror("fopen");
		exit(1);
	}
	fprintf(fp, "num\t\tname\t\tscore\n");
	while (pb!=NULL)
	{
		fprintf(fp,"%d\t\t%s\t\t%d\n", pb->num, pb->name, pb->score);
		pb = pb->next;
	}
	fclose(fp);
}
// �˳�
void exit_fun(ppTYPE phead, char *cmd)
{
	//�ͷ���������нڵ�
	save_file(DATAFILE, *phead);
	free_link(*phead);
	exit(1);
}

// ��������
void cls_fun(ppTYPE phead, char *cmd)
{
	system("cls"); 
}

//����ָ��
typedef void (*FUN)(ppTYPE phead, char *cmd);

//����ṹ��
typedef struct cmd
{
	char *name;		//��������
	FUN fun;		//�������
}CMD;

//�����˵�
char *help= "*******************************************\n"\
			"* add         :    ���ѧ����Ϣ           *\n"\
			"* del name    :    ������ɾ��             *\n"\
			"* del num     :    ��ѧ��ɾ��             *\n"\
			"* print       :    ��ӡ������Ϣ           *\n"\
			"* find name   :    ����������             *\n"\
			"* find num    :    ��ѧ�Ų���             *\n"\
			"* order num   :    ��ѧ������             *\n"\
			"* order score :    ���ɼ�����             *\n"\
			"* cls         :    ��ƽ                   *\n"\
			"* help        :    ����                   *\n"\
			"* exit        :    �Ƴ�                   *\n"\
			"*******************************************\n"\
			;


//�������飺�������� ������ �� ��������
CMD cmdlist[]={		
				{"add", add_fun},		// �������
				{"del", del_fun},		// ɾ������
				{"print", print_fun},	// ��ӡ����
				{"find", find_fun},		// ��������
				{"order", order_fun},	// ��������
				{"help", help_fun},		// ��������
				{"exit", exit_fun},		// �˳�
				{"cls", cls_fun}		// ��������
			};


pTYPE init_data(ppTYPE phead, char *file)
{
	FILE *fp = NULL;
	pTYPE pb = NULL;
	int num = 0;
	char name[20]="";
	int score = 0;
	char buf[1024]="";

	if ((fp=fopen(file, "r"))==0)
	{
		perror("fopen");
		exit(1);
	}
	fgets(buf, sizeof(buf), fp);
	do 
	{
		fscanf(fp,"%d\t%s\t%d", &num, name, &score);
		if (feof(fp)!=0)
		{
			break;
		}
		pb = (pTYPE)malloc(sizeof(TYPE));
		pb->num = num;
		pb->score = score;
		strcpy(pb->name, name);
		pb->next = NULL;
		pb->prev = NULL;
		*phead = add_node(phead, pb);
	} while (1);
	fclose(fp);
	return *phead;
}

int main()
{
	pSTU head=NULL;
	head = init_data(&head,DATAFILE);
	printf("%s", help);
	while (1)
	{
		int i = 0;
		char cmd[100]="";
		printf("ѧ����Ϣ����>:");
		fgets(cmd, sizeof(cmd), stdin);			//��������
		cmd[strlen(cmd)-1] = '\0';
		if(strlen(cmd)==0)
			continue;
		for (i=0;i<sizeof(cmdlist)/sizeof(CMD);i++)
		{
			//��������
			if (strncmp(cmdlist[i].name, cmd, strlen(cmdlist[i].name))==0)
			{
				//ִ������
				cmdlist[i].fun(&head, cmd);
			}
		}		
	}
	return 0;
}