#include "link.h"
int main()
{
	STU *head1=NULL,*p_new=NULL,*result;
	int i,num,cmd;
	char name[20],flag;
	help();
	while(1)
	{
		printf("请输入你要求的命令,0帮助:\n");
		scanf("%d",&cmd);
		switch(cmd)
		{
			case 0:
				system("cls");
				help();
				break;
			case 1:
				printf("请输入链表初始个数:\n");
				scanf("%d",&num);
				for(i=0;i<num;i++)
				{	
					p_new=(STU*)malloc(sizeof(STU));//申请一个新节点
					printf("请输入学号、分数、名字:\n");	//给新节点赋值
					scanf("%d %f %s",&p_new->num,&p_new->score,p_new->name);
					link_creat_head(&head1,p_new);	//将新节点加入链表
				}
				break;
			case 2:
				print_link(head1);
				break;
				
			case 3:
				printf("请输入你要查找的名字:\n");
				scanf("%s",name);
				result =search_name_link(head1,name);
				if(result==NULL)
					printf("没找到你要的节点");
				else
					printf("%d %f %s\n",result->num,result->score,result->name);
				break;
				
			case 4:
				printf("请输入你要查找的学号:\n");
				scanf("%d",&num);
				result =search_num_link(head1,num);
				if(result==NULL)
					printf("没找到你要的节点");
				else
					printf("%d %f %s\n",result->num,result->score,result->name);
				break;
			case 5:
				printf("请输入你要删除的学号信息:\n");
				scanf("%d",&num);
				flag =delet_num_link(&head1,num);
				if(flag==0)
					printf("没找到你要删的节点");
				else
					printf("删除成功\n");
				break;
			case 6:
				break;
			case 7:
				printf("请输入你要插入的学号信息:\n");
				p_new=(STU*)malloc(sizeof(STU));
				scanf("%d %f %s",&p_new->num,&p_new->score,p_new->name);
				insert_num_link(&head1,p_new);
				break;
			case 8:
				break;
			case 9:
				select_order_link(head1);
				break;
			case 10:
				free_link(&head1);
			case 11:
				reverse_link(&head1);
				
				break;			
			}
	}
	return 0;
}