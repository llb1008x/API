#ifndef ARP_LINK_H
#define ARP_LINK_H

#include "types.h"

/******************************************************************
函	数:	void link_creat_head(ARP_CACHE_LIST **head,ARP_CACHE_LIST *p_new)
功	能:	创建ARP缓存表
参	数:	ARP_CACHE_LIST **head ARP链表头  ARP_CACHE_LIST *p_new 新节点
返回值: 无
*******************************************************************/
extern void link_creat_head(ARP_CACHE_LIST **head,ARP_CACHE_LIST *p_new);

/******************************************************************
函	数:	void link_print(ARP_CACHE_LIST *head)
功	能:	打印ARP缓存表
参	数:	ARP_CACHE_LIST *head arp链表头
返回值: 无
*******************************************************************/
extern void link_print(ARP_CACHE_LIST *head);

/******************************************************************
函	数:	ARP_CACHE_LIST *link_search_ip(ARP_CACHE_LIST *head,unsigned char *ip)
功	能:	查找arp缓存表
参	数:	ARP_CACHE_LIST *head arp链表头  unsigned char *ip 待查找IP
返回值: ARP_CACHE_LIST *找到的节点
*******************************************************************/
extern ARP_CACHE_LIST *link_search_ip(ARP_CACHE_LIST *head,unsigned char *ip);

#endif