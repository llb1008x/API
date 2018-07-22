#include "types.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/******************************************************************
函	数:	void link_creat_head(ARP_CACHE_LIST **head,ARP_CACHE_LIST *p_new)
功	能:	创建ARP缓存表
参	数:	ARP_CACHE_LIST **head ARP链表头  ARP_CACHE_LIST *p_new 新节点
返回值: 无
*******************************************************************/
void link_creat_head(ARP_CACHE_LIST **head,ARP_CACHE_LIST *p_new)
{
	ARP_CACHE_LIST *p_mov=*head;
	
	if(*head==NULL)
	{
		*head=p_new;
		p_new->next=NULL;
	}
	else
	{
		while(p_mov->next!=NULL)
		{
			p_mov=p_mov->next;
		}
		p_mov->next=p_new;
		p_new->next=NULL;
	}
}

/******************************************************************
函	数:	void link_print(ARP_CACHE_LIST *head)
功	能:	打印ARP缓存表
参	数:	ARP_CACHE_LIST *head arp链表头
返回值: 无
*******************************************************************/
void link_print(ARP_CACHE_LIST *head)
{
	printf("---------arp_link_top----------\n");
	ARP_CACHE_LIST *p_mov=head;
	while(p_mov!=NULL)
	{
		printf("IP=%d.%d.%d.%d\n",p_mov->ip[0],p_mov->ip[1],p_mov->ip[2],p_mov->ip[3]);
		printf("MAC=%02x:%02x:%02x:%02x:%02x:%02x\n",
				p_mov->mac[0],p_mov->mac[1],p_mov->mac[2],
				p_mov->mac[3],p_mov->mac[4],p_mov->mac[5]);
		p_mov=p_mov->next;
	}
	printf("---------arp_link_end----------\n");
}

/******************************************************************
函	数:	ARP_CACHE_LIST *link_search_ip(ARP_CACHE_LIST *head,unsigned char *ip)
功	能:	查找arp缓存表
参	数:	ARP_CACHE_LIST *head arp链表头  unsigned char *ip 待查找IP
返回值: ARP_CACHE_LIST *找到的节点
*******************************************************************/
ARP_CACHE_LIST *link_search_ip(ARP_CACHE_LIST *head,unsigned char *ip)
{
	ARP_CACHE_LIST *p_mov=head;
	while(p_mov!=NULL)
	{
		if(memcmp(p_mov->ip,ip,4)==0)	/*用memcpy指定要比较多少个字节*/
		{
			return p_mov;
		}
		p_mov=p_mov->next;
	}
	return NULL;
}