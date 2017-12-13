#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "student.h"

#define DATAFILE	"stu.txt"

//help的声明, 在后边定义
char *help;

// 添加命令
void add_fun(ppTYPE phead, char *cmd)
{	
	pTYPE pb = (pTYPE)malloc(sizeof(TYPE));
	printf("请输入要添加的学生信息\n(学号 姓名 成绩)\n");
	scanf("%d %s %d", &pb->num, pb->name, &pb->score);
	pb->next = NULL;
	pb->prev = NULL;
	getchar();
	//将pb所指的学生信息添加到链表中
	add_node(phead, pb);			
}

// 删除命令
void del_fun(ppTYPE phead, char *cmd)
{
	int temp = 0;
	//p指向del后的参数，后边用来判断 是"neme" 还是 "num"
	//"neme" 和 "num" 分别表示根据 姓名 还是根据 学号 删除
	char *p = cmd+4;
	pTYPE pb = NULL;	
	if (strncmp(p, "name", 4)==0)	//根据姓名删除
	{
		char name[20]="";
		printf("请输入姓名:");
		scanf("%s", name);
		getchar();
		pb = find_node(*phead, FIND_BY_NAME, (void*)name);	//找到姓名为name的学生信息
	}
	else if(strncmp(p, "num", 3)==0) //根据学号删除
	{
		int num = -1;
		printf("请输入学号:");
		scanf("%d", &num);
		getchar();
		pb = find_node(*phead, FIND_BY_NUM, (void*)num);	//找到姓名为name的学生信息
	}
	if (pb!=NULL)
	{
		*phead = del_node(phead, pb);	//删除上边找到的学生
	}
	else
	{
		printf("查无此人!\n");
	}
}

// 查找命令
void find_fun(ppTYPE phead, char *cmd)
{
	char *p = cmd+5;
	//p指向find后的参数，后边用来判断 是"num" 还是 "name"
	//"num" 和 "name" 分别表示根据 学号 还是根据 姓名 来查找
	pTYPE pb = NULL;
	pTYPE ret = NULL;
	
	if (strncmp(p, "name", 4)==0)			//根据 姓名 
	{
		char name[20]="";
		printf("请输入要查找的学生姓名:");
		scanf("%s", name);
		getchar();
		ret = find_node(*phead, FIND_BY_NAME, name);	//查找节点
	} 
	else if(strncmp(p, "num", 3)==0)	    //根据 学号 
	{
		int num = 0;
		printf("请输入要查找的学生学号:");
		scanf("%d", &num);
		getchar();
		ret = find_node(*phead, FIND_BY_NUM, p);        //查找节点
	}
	if (ret !=NULL)
	{	//打印找到的学生信息
		printf("\t%4d\t%3s\t%4d\n", ret->num, ret->name, ret->score);
	}
	else
	{
		printf("查无此人!\n");
	}
}

// 打印命令
void print_fun(ppTYPE phead, char *cmd)
{
	// 遍历链表，并打印
	print_link(*phead);
}

// 排序命令
void order_fun(ppTYPE phead, char *cmd)
{
	char *p = cmd+6;
	//p指向order后的参数，后边用来判断 是"num" 还是 "score"
	//"num" 和 "score" 分别表示根据 学号 还是根据 成绩 来排序
	pTYPE pb = NULL;
	if (strncmp(p, "score", 4)==0)		 	 //根据 成绩
	{
		order_link(*phead, ORDER_BY_SCORE);	 // 排序 
	}
	else if(strncmp(p, "num", 3)==0)		 //根据 学号
	{
		order_link(*phead, ORDER_BY_NUM);	 // 排序 
	}
}


// 帮助命令
void help_fun(ppTYPE phead, char *cmd)
{
	//打印帮助菜单
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
// 退出
void exit_fun(ppTYPE phead, char *cmd)
{
	//释放链表的所有节点
	save_file(DATAFILE, *phead);
	free_link(*phead);
	exit(1);
}

// 清屏命令
void cls_fun(ppTYPE phead, char *cmd)
{
	system("cls"); 
}

//函数指针
typedef void (*FUN)(ppTYPE phead, char *cmd);

//命令结构体
typedef struct cmd
{
	char *name;		//命令名称
	FUN fun;		//命令处理函数
}CMD;

//帮助菜单
char *help= "*******************************************\n"\
			"* add         :    添加学生信息           *\n"\
			"* del name    :    按姓名删除             *\n"\
			"* del num     :    按学号删除             *\n"\
			"* print       :    打印所有信息           *\n"\
			"* find name   :    按姓名查找             *\n"\
			"* find num    :    按学号查找             *\n"\
			"* order num   :    按学号排序             *\n"\
			"* order score :    按成绩排序             *\n"\
			"* cls         :    清平                   *\n"\
			"* help        :    帮助                   *\n"\
			"* exit        :    推出                   *\n"\
			"*******************************************\n"\
			;


//命令数组：用来保存 命令名 和 处理函数名
CMD cmdlist[]={		
				{"add", add_fun},		// 添加命令
				{"del", del_fun},		// 删除命令
				{"print", print_fun},	// 打印命令
				{"find", find_fun},		// 查找命令
				{"order", order_fun},	// 排序命令
				{"help", help_fun},		// 帮助命令
				{"exit", exit_fun},		// 退出
				{"cls", cls_fun}		// 清屏命令
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
		printf("学生信息管理>:");
		fgets(cmd, sizeof(cmd), stdin);			//输入命令
		cmd[strlen(cmd)-1] = '\0';
		if(strlen(cmd)==0)
			continue;
		for (i=0;i<sizeof(cmdlist)/sizeof(CMD);i++)
		{
			//查找命令
			if (strncmp(cmdlist[i].name, cmd, strlen(cmdlist[i].name))==0)
			{
				//执行命令
				cmdlist[i].fun(&head, cmd);
			}
		}		
	}
	return 0;
}