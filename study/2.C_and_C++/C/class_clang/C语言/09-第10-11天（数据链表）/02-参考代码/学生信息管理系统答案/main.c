#include "link.h"
int main()
{
	STU *head1=NULL,*p_new=NULL,*result;
	int i,num,cmd;
	char name[20],flag;
	help();
	while(1)
	{
		printf("��������Ҫ�������,0����:\n");
		scanf("%d",&cmd);
		switch(cmd)
		{
			case 0:
				system("cls");
				help();
				break;
			case 1:
				printf("�����������ʼ����:\n");
				scanf("%d",&num);
				for(i=0;i<num;i++)
				{	
					p_new=(STU*)malloc(sizeof(STU));//����һ���½ڵ�
					printf("������ѧ�š�����������:\n");	//���½ڵ㸳ֵ
					scanf("%d %f %s",&p_new->num,&p_new->score,p_new->name);
					link_creat_head(&head1,p_new);	//���½ڵ��������
				}
				break;
			case 2:
				print_link(head1);
				break;
				
			case 3:
				printf("��������Ҫ���ҵ�����:\n");
				scanf("%s",name);
				result =search_name_link(head1,name);
				if(result==NULL)
					printf("û�ҵ���Ҫ�Ľڵ�");
				else
					printf("%d %f %s\n",result->num,result->score,result->name);
				break;
				
			case 4:
				printf("��������Ҫ���ҵ�ѧ��:\n");
				scanf("%d",&num);
				result =search_num_link(head1,num);
				if(result==NULL)
					printf("û�ҵ���Ҫ�Ľڵ�");
				else
					printf("%d %f %s\n",result->num,result->score,result->name);
				break;
			case 5:
				printf("��������Ҫɾ����ѧ����Ϣ:\n");
				scanf("%d",&num);
				flag =delet_num_link(&head1,num);
				if(flag==0)
					printf("û�ҵ���Ҫɾ�Ľڵ�");
				else
					printf("ɾ���ɹ�\n");
				break;
			case 6:
				break;
			case 7:
				printf("��������Ҫ�����ѧ����Ϣ:\n");
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