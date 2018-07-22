#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "ip_link.h"

IP_LINK *ip_head=NULL;

/***********************************************
函数功能:	把链表数据写入文件
参    数:	无
返 回 值:	无
***********************************************/
void write_ip_link()
{
	FILE *ip_config = fopen("ip_filter_link", "wb+");//以二进制可写方式打开文件
	if(ip_config == NULL)
	{
		perror("error");
		exit(1);
	}
	char buff[20] = "";
	IP_LINK *pb = ip_head;
	//把ip过滤信息写入文件
	while(pb != NULL)
	{
		sprintf(buff,"%d.%d.%d.%d\n",pb->ip[0], pb->ip[1], pb->ip[2], pb->ip[3]);//组ip包
		fputs(buff, ip_config);//把ip写入文件
		pb = pb->next;
	}

	fclose(ip_config);//关闭文件
}

/*********************************************
函数功能:	从文件中读取链表数据
参    数:	无
返 回 值:	无
*********************************************/
void read_ip_link()
{
	FILE *ip_config = NULL;
	ip_config = fopen("ip_filter_link", "rb+");//以二进制可读方式打开文件
	if(ip_config == NULL)
	{
		perror("error");
		exit(1);
	}
	while(1)
	{
		char buff[500]="";
		bzero(buff,sizeof(buff));
		int ip;
		if(fgets(buff, sizeof(buff), ip_config) == NULL)//从文件中读取ip
		{
			break;
		}
		buff[strlen(buff)-1] = '\0';
		inet_pton(AF_INET, buff, &ip);
		IP_LINK *pb = (IP_LINK *)malloc(sizeof(IP_LINK));	//创建ip链表
		memcpy(pb->ip, &ip, 4);
		ip_head = insert_ip_link(ip_head, pb);//插入链表
	}
	print_ip(ip_head);//打印ip过滤链表

	fclose(ip_config);//关闭文件
}

/******************************************************************
函数功能:	插入ip到过滤链表
参    数:	IP_LINK *head ip过滤链表头 ，IP_LINK* p 待插入节点
返 回 值:	链表首地址
*******************************************************************/
IP_LINK *insert_ip_link(IP_LINK *head, IP_LINK* p)
{
	IP_LINK *temp = find_ip(head, p->ip);//查找ip
	/*如果是未知ip ，插入链表*/
	if(temp == NULL)
	{
		p->next = head;//在链表头处插入节点
		head = p;
	}
	else
	{
		free(p);
	}
	return head;
}

/******************************************************************
函数功能:	插入mac到过滤链表
参    数:	IP_LINK *head ip过滤链表头 ，IP_LINK* p 待插入节点
返 回 值:	链表首地址
*******************************************************************/
IP_LINK *insert_mac_link(IP_LINK *head, IP_LINK *p)
{
	IP_LINK *temp = find_mac(head, p->mac);//查找mac
	/*如果是未知mac ，插入链表*/
	if(temp == NULL)
	{
		p->next = head;//在链表头处插入节点
		head = p;
	}
	else
	{
		free(p);	//若存在，则释放待插入节点
	}
	return head;
}

/******************************************************************
函数功能:	查找ip
参    数:	IP_LINK *head ip过滤链表头 ，待查找的ip
返 回 值:	找到的节点地址
*******************************************************************/
IP_LINK *find_ip(IP_LINK *head, unsigned char *ip)
{
	IP_LINK *temp = head;
	while(temp)
	{
		if(memcmp(temp->ip, ip, 4) == 0)//对比ip
		{
			break;
		}
		temp = temp->next;	//指向下一节点
	}
	return temp;
}

/******************************************************************
函数功能:	查找mac
参    数:	IP_LINK *head ip过滤链表头 ，待查找的mac地址
返 回 值:	找到的节点地址
*******************************************************************/
IP_LINK *find_mac(IP_LINK *head, unsigned char *mac)
{
	IP_LINK *temp = head;
	while(temp)
	{
		if(memcmp(temp->mac, mac, 6) == 0)//对比mac
		{
			break;
		}
		temp = temp->next;	//指向下一节点
	}
	return temp;
}

/******************************************************************
函数功能:	打印ip过滤链表信息
参    数:	IP_LINK *head ip过滤链表头
返 回 值:	无
*******************************************************************/
void print_ip(IP_LINK *head)
{
	IP_LINK *temp = head;
	printf("防火墙ip过滤链表信息：\n");
	//循环打印链表信息
	while(temp != NULL)
	{
		printf("ip: %d.%d.%d.%d\n",
			temp->ip[0], temp->ip[1], temp->ip[2], temp->ip[3]//ip地址
			);
		temp = temp->next;//指向下一节点
	}
}

/******************************************************************
函数功能:	打印mac过滤链表信息
参    数:	IP_LINK *head ip过滤链表头
返 回 值:	无
*******************************************************************/
void print_mac(IP_LINK *head)
{
	IP_LINK *pb = head;
	printf("防火墙mac过滤链表信息：\n");
	//循环打印链表信息
	while(pb != NULL)
	{
		printf("%s\n", pb->mac);
		pb = pb->next;
	}
}

/******************************************************************
函数功能:	删除ip过滤链表节点
参    数:	IP_LINK *head ip 过滤链表头，unsigned char *ip 待删除IP
返 回 值:	IP_LINK *head ip 过滤链表头
*******************************************************************/
IP_LINK *delete_ip(IP_LINK *head, unsigned char *ip)
{
	IP_LINK *pf,*pb;
	pf = pb = head;
	/*查找待删除ip 节点*/
	while(pb)
	{
		if(memcmp(pb->ip,ip,4) == 0)
		{
			break;
		}
		pf = pb;
		pb = pb->next;
	}
	if(pb != NULL)
	{
		/*待删除ip 位于头部*/
		if(pb == head)
		{
			head = head->next;
		}
		/*待删除ip 不在头部*/
		else
		{
			pf->next = pb->next;
		}
		free(pb);
		pb = NULL;
	}
	return head;
}

/******************************************************************
函数功能:	删除mac过滤链表节点
参    数:	IP_LINK *head ip 过滤链表头，unsigned char *mac 待删除mac
返 回 值:	IP_LINK *head ip 过滤链表头
*******************************************************************/
IP_LINK *delete_mac(IP_LINK *head, unsigned char *mac)
{
	IP_LINK *pf,*pb;
	pf = pb = head;
	/*查找待删除mac节点*/
	while(pb)
	{
		if(memcmp(pb->mac, mac, 6) == 0)
		{
			break;
		}
		pf = pb;
		pb = pb->next;
	}
	if(pb != NULL)
	{
		/*待删除mac位于头部*/
		if(pb == head)
		{
			head = head->next;
		}
		/*待删除mac不在头部*/
		else
		{
			pf->next = pb->next;
		}
		free(pb);
		pb = NULL;
	}
	return head;
}

/******************************************************************
函数功能:	释放ip 过滤链表
参    数:	IP_LINK *head ip过滤链表头
返 回 值:	无
*******************************************************************/
void free_ip_link(IP_LINK *head)
{
	IP_LINK *pb = head;
	while(head)
	{
		pb = head->next;
		free(head);
		head = pb;
	}
}
