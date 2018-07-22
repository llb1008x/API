#include <stdlib.h>
#include "arp_link.h"


//ARP缓存表链表首地址
ARP *arp_head=NULL;

/************************************************************************
函	数:	ARP *arp_link_creat( ARP *p_head, ARP *p_new )
功  能: 创建ARP缓存表链表
参	数:	p_head：ARP缓存表链表首地址, p_new：新节点地址
返回值：p_head(ARP缓存表链表首地址)
*************************************************************************/
ARP *arp_link_creat( ARP *p_head, ARP *p_new )
{
	ARP *p_mov = p_head;
	if( p_mov == NULL )       //第一个节点为空
	{
		p_head = p_new;       //将新的节点地址赋给链表首地址
		p_new->next = NULL;
	}
	else 
	{
		if(arp_search_ip(p_head, (unsigned char*)p_new) != NULL)
		{
			return p_head;
		}
		else
		{
			while((	arp_search_ip(p_head, (unsigned char*)p_new) == NULL) && (p_mov->next != NULL))       //当前节点的后向不为空就继续循环
			{
				p_mov = p_mov->next;         //向后移动到下一个节点
			}
			p_mov->next = p_new;             //将新节点地址赋给当前节点的后面
		}
	}
	return p_head;
}

/************************************************************************
函	数:	ARP *arp_search_ip( ARP *p_head, unsigned char *ip )
功	能:	按IP地址查找ARP缓存表节点
参	数:	p_head：ARP缓存表链表的首地址, ip：32位无符号整数IP地址的指针
返回值：找到返回节点指针，找不到返回NULL
*************************************************************************/
ARP *arp_search_ip( ARP *p_head, unsigned char *ip )
{
	ARP *p_mov = p_head;
	while( p_mov != NULL )               //当前节点不为空就继续循环
	{
		if( *(unsigned int *)(p_mov->ip) == *(unsigned int*)ip )    //若果当前节点的IP等于要找的IP
			return p_mov;                                 	        //返回当前节点
		p_mov = p_mov->next;                                        //否则向后移动继续查找下一个节点
	}
	return NULL;                    //找不到返回NULL
}

/************************************************************************
函	数:	void arp_print( ARP *p_head )
功	能:	遍历ARP缓存表链表
参	数: p_head：ARP缓存表链表的首地址
返回值：无
*************************************************************************/
void arp_print( ARP *p_head )
{
	ARP *p_mov = p_head;
	printf("路由器缓存表信息：\n");
	while(p_mov != NULL)              //当前节点不为空就继续循环
	{
		printf("IP:%d.%d.%d.%d   >>  MAC:%02x:%02x:%02x:%02x:%02x:%02x\n",
			    p_mov->ip[0],p_mov->ip[1],p_mov->ip[2],p_mov->ip[3],
			    p_mov->mac[0],p_mov->mac[1],p_mov->mac[2],p_mov->mac[3],
			    p_mov->mac[4],p_mov->mac[5]);            //打印IP地址和MAC地址
		p_mov = p_mov->next;         //继续查找下一个节点
	}
}

	
	
