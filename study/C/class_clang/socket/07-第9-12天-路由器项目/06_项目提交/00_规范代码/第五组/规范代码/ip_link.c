#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ip_link.h"
#define    ip_config_name  "ip.txt"
IP_LINK *ip_head = NULL;
//保持ip到文件
void save_ip_link()
{

	FILE *pf = fopen( ip_config_name,"wb+");
	if(pf==NULL)
	{
		perror("connot open file");
		exit(-1);
	}
	char buff[20]="";
	IP_LINK *pb=ip_head;
	while(pb!=NULL)
	{
		sprintf(buff,"%d.%d.%d.%d",pb->ip[0],pb->ip[1],pb->ip[2],pb->ip[3]);
		fputs(buff,pf);
		pb  = pb->next;
	}
	fclose(pf);
}

IP_LINK *find_ip(IP_LINK *head,unsigned char *ip)
{
	IP_LINK *pb = head;
	while(pb!=NULL)
	{
		if(memcmp(pb->ip,ip,4)==0)
			return pb;
		pb = pb->next;
	}
	return NULL;
}


//将输入的ip放入链表
void inner_ip_link(IP_LINK **head,IP_LINK *p)
{
	IP_LINK *pb = *head;
	if(*head==NULL)				//没有找到就插入头部
	{
		*head = p;
		p->next = NULL;
	}
	else
	{
		while(pb->next != NULL && memcmp(p->ip,pb->ip,4) != 0)
		{
			pb = pb->next;
		}
		if(memcmp(p->ip,pb->ip,4) == 0)
		{
			return ;
		}
		if(pb->next == NULL)
		{
			pb->next = p;
			p->next =NULL;
		}
	}
	
}

void del_ip_link(IP_LINK *head)
{
	printf("please input the ip that you want to del\n");
	char delete[16]="";
	fgets(delete,sizeof(delete),stdin);
	delete[strlen(delete)-1]=0;
	int ip=0;
	inet_pton(AF_INET,delete,&ip);
	
	unsigned char dell[4]="";
	memcpy(dell,&ip,4);
	IP_LINK *pb;
	pb=head;
	
	if(memcmp(pb->ip,dell,4)==0)
	{
		head = head->next;
		free(pb);
		
	}

	
}
void printf_ip_link(IP_LINK *head)
{
	printf("\n---------ip_link_start----------\n");
	IP_LINK *pb = head;
	while(pb!=NULL){
		printf("%d.%d.%d.%d\n", pb->ip[0],pb->ip[1],pb->ip[2],pb->ip[3]);
		pb = pb->next;
	}
	printf("-------------------------------\n\n");
}

//从键盘得到ip放入结构体
void set_ip()
{	
	printf("please input ip\n");
	FILE *ip_config = NULL;
	ip_config = fopen( ip_config_name,"wb+");
	
		char buff[100]="";
		bzero(buff,sizeof(buff));
		//fgets(buff,sizeof(buff),ip_config)	;
		fgets(buff,sizeof(buff),stdin);
		buff[strlen(buff)-1]=0;
		fwrite(buff,strlen(buff),1,ip_config);
		//save_ip_link();
		IP_LINK *p_new = (IP_LINK *)malloc(sizeof(IP_LINK));
		inet_pton(AF_INET,buff,p_new->ip);
		inner_ip_link(&ip_head,p_new);	
	
	fclose(ip_config);
	
}
