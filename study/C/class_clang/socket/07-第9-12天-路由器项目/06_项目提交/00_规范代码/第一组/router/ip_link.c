#include "types.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ip_config_name "ip_config"
extern void printf_ip_link(IP_FILTER_LIST *head);
extern IP_FILTER_LIST *insert_ip_link(IP_FILTER_LIST *head,IP_FILTER_LIST* p);

/******************************************************************
函	数:	void save_ip_link(void)
功	能:	保存链表数据到配置文件
参	数:	无
返回值: 无
*******************************************************************/
void save_ip_link(void)
{
	FILE *ip_fd=fopen(ip_config_name,"wb+");
	if(ip_fd==NULL)
	{
		perror("error");
		_exit(1);
	}
	char buf[20]="";
	IP_FILTER_LIST *pb=ip_head;
	while(pb!=NULL)
	{
		sprintf(buf,"%d.%d.%d.%d\n",pb->ip[0],pb->ip[1],pb->ip[2],pb->ip[3]);
		fputs(buf,ip_fd);
		pb=pb->next;
	}
	fclose(ip_fd);
}

/******************************************************************
函	数:	void read_ip_config(void)
功	能:	读取配置文件数据到链表
参	数:	无
返回值: 无
*******************************************************************/
void read_ip_config(void)
{
	FILE *ip_fd=fopen(ip_config_name,"rb+");
	if(ip_fd == NULL)
	{
		perror("error");
		_exit(1);
	}
	while(1)
	{
		char buf[500]="";
		bzero(buf,sizeof(buf));
		unsigned int ip;
		if(fgets(buf,sizeof(buf),ip_fd)==NULL)
		{
			break;
		}
		buf[strlen(buf)-1]=0;	//注意文件中存在\r
		inet_pton(AF_INET,buf,&ip);
		IP_FILTER_LIST *pb =(IP_FILTER_LIST *)malloc(sizeof(IP_FILTER_LIST));	
		memcpy(pb->ip,&ip,4);
		ip_head=insert_ip_link(ip_head,pb);
	}
	//printf_ip_link(ip_head);
	fclose(ip_fd);
}

/******************************************************************
函	数:	void free_ip_link(IP_FILTER_LIST *head)
功	能:	释放ip过滤链表
参	数:	IP_FILTER_LIST *head ip过滤链表头
返回值: 无
*******************************************************************/
void free_ip_link(IP_FILTER_LIST *head)
{
	IP_FILTER_LIST *pb=head;
	while(head)
	{
		pb=head->next;
		free(head);
		head=pb;
	}
}

/******************************************************************
函	数:	void printf_ip_link(IP_FILTER_LIST *head)
功	能:	打印ip过滤链表
参	数:	IP_FILTER_LIST *head ip过滤链表头
返回值: 无
*******************************************************************/
void printf_ip_link(IP_FILTER_LIST *head)
{
	printf("---------ip_link_top----------\n");
	IP_FILTER_LIST *pb=head;
	while(pb!=NULL)
	{
		printf("%d.%d.%d.%d\n",pb->ip[0],pb->ip[1],pb->ip[2],pb->ip[3]);
		pb=pb->next;
	}
	printf("---------ip_link_end----------\n");
}

/******************************************************************
函	数:	IP_FILTER_LIST *find_ip(IP_FILTER_LIST *head, unsigned char *ip)
功	能:	查找ip过滤链表
参	数:	IP_FILTER_LIST *head ip过滤链表头  unsigned char *ip 待查找IP
返回值: IP_FILTER_LIST *找到的节点
*******************************************************************/
IP_FILTER_LIST *find_ip(IP_FILTER_LIST *head, unsigned char *ip)
{
	IP_FILTER_LIST *pb = head;
	while(pb)
	{
		if(memcmp(pb->ip,ip,4)==0)
		{
			break;
		}
		pb=pb->next;
	}
	return pb;
}

/******************************************************************
函	数:	IP_FILTER_LIST *insert_ip_link(IP_FILTER_LIST *head, unsigned char *ip)
功	能:	插入ip过滤链表
参	数:	IP_FILTER_LIST *head ip过滤链表头  IP_FILTER_LIST* p 待插入节点
返回值: IP_FILTER_LIST *找到的节点
*******************************************************************/
IP_FILTER_LIST *insert_ip_link(IP_FILTER_LIST *head,IP_FILTER_LIST *p)
{
	IP_FILTER_LIST *pb=find_ip(head,p->ip);	//查找是否有该记录
	if(pb==NULL)
	{
		p->next=head;	//未查找到，插入链表，直接插入表头方便
		head=p;
	}
	else
	{
		free(p);
	}
	return head;
}

/******************************************************************
函	数:	IP_FILTER_LIST *del_ip_for_link(IP_FILTER_LIST *head, unsigned char *ip)
功	能:	删除ip过滤链表节点
参	数:	IP_FILTER_LIST *head ip过滤链表头  unsigned char *ip 待删除IP
返回值: IP_FILTER_LIST *head ip过滤链表头
*******************************************************************/
IP_FILTER_LIST *del_ip_for_link(IP_FILTER_LIST *head, unsigned char *ip)
{
	IP_FILTER_LIST *pf,*pb;
	pf=pb=head;
	while(pb)
	{
		if(memcmp(pb->ip,ip,4)==0)
		{
			break;
		}
		pf=pb;
		pb=pb->next;
	}
	if(pb!=NULL)
	{
		if(pb==head)
		{
			head=head->next;
		}
		else
		{
			pf->next=pb->next;
		}
		free(pb);
		pb=NULL;
	}
	return head;
}