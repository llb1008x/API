#ifndef ARP_LINK_H
#define ARP_LINK_H

/*ARP 链表结构体*/
typedef struct arp_link{
	unsigned char arp_ip[4];	//ip地址
	unsigned char arp_mac[6];	//mac地址
	
	struct arp_link *next;		//下一节点
}ARP_LINK;

/*************************************************************
函数功能:	创建一个链表，或在其尾部插入节点
参    数:	ARP链表头地址，节点指针
返 回 值:	ARP链表头地址
*************************************************************/
extern void insert_link(ARP_LINK **head, ARP_LINK *p_new);

/**********************************
函数功能:	遍历链表内容
参    数:	ARP链表头地址
返 回 值:	无
**********************************/
extern void print_link(ARP_LINK *head);

/*******************************************
函数功能:	根据ip查找mac
参    数:	ARP链表头地址，待查找ip的地址
返 回 值:	查找到节点的地址
*******************************************/
extern ARP_LINK *ip_find_mac(ARP_LINK *head, unsigned char *ip);

/***********************************************
函数功能:	释放ARP链表
参    数:	ARP链表头地址
返 回 值:	无
***********************************************/
extern void free_arp_link(ARP_LINK *head);

#endif