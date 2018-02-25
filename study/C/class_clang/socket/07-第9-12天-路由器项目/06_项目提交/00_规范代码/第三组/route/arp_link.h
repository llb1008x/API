#ifndef __ARP_LINK_H__
#define __ARP_LINK_H__

#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/ether.h>
#include <string.h>
#include <sys/ioctl.h>
#include <netpacket/packet.h>
#include <stdlib.h>
#include <net/if.h>
#include <pthread.h>
#include <unistd.h>					
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>

#include "arp_link.h"


//定义ARP缓存表结构体
typedef struct arp_link
{
	unsigned char ip[4];			//IP地址
	unsigned char mac[30];		    //MAC地址
	struct arp_link *next;		    //链表后项地址
}ARP;

//声明ARP缓存表
extern ARP * arp_head;


/************************************************************************
函	数:	ARP *arp_link_creat( ARP *p_head, ARP *p_new )
功  能: 创建ARP缓存表链表
参	数:	p_head：ARP缓存表链表首地址, p_new：新节点地址
返回值: p_head(ARP缓存表链表首地址)
*************************************************************************/
extern ARP *arp_link_creat( ARP *p_head, ARP *p_new );


/************************************************************************
函	数:	ARP *arp_search_ip( ARP *p_head, unsigned char *ip )
功	能:	按IP地址查找ARP缓存表节点
参	数:	p_head：ARP缓存表链表的首地址, ip：32位无符号整数IP地址的指针
返回值：找到返回节点指针，找不到返回NULL
*************************************************************************/
extern ARP* arp_search_ip( ARP *p_head,unsigned char *ip );


/************************************************************************
函	数:	void arp_print( ARP *p_head )
功	能:	遍历ARP缓存表链表
参	数: p_head：ARP缓存表链表的首地址
返回值：无
*************************************************************************/
extern void arp_print( ARP *p_head );


#endif