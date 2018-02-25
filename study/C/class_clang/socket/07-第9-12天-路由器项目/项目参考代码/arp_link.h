#ifndef ARP_LINK_H
#define ARP_LINK_H

typedef struct arp_link{
	unsigned char ip[4];//ip地址四个字节
	unsigned char mac[6];//mac地址六个字节
	struct arp_link *next;
}ARP_LINK;
extern ARP_LINK *arp_head;

/******************************************************************
函	数:	ARP_LINK *inner_arp_link(ARP_LINK *head,ARP_LINK* p)
功	能:	插入ARP链表
参	数:	ARP_LINK *head ARP链表头  unsigned char *ip IP地址
返回值: ARP_LINK *head ARP链表头
*******************************************************************/
extern ARP_LINK *inner_arp_link(ARP_LINK *arp_head,ARP_LINK* p);

/******************************************************************
函	数:	void printf_arp_link(ARP_LINK *head)
功	能:	打印ARP链表
参	数:	ARP_LINK *head ARP链表头
返回值: 无
*******************************************************************/
extern void printf_arp_link(ARP_LINK *arp_head);

/******************************************************************
函	数:	ARP_LINK *find_arp_from_ip(ARP_LINK *head, unsigned char *ip)
功	能:	根据IP查找MAC
参	数:	ARP_LINK *head ARP链表头  unsigned char *ip IP地址
返回值: ARP_LINK *查找到的节点
*******************************************************************/
extern ARP_LINK *find_arp_from_ip(ARP_LINK *arp_head, unsigned char *ip);
/******************************************************************
函	数:	void free_arp_link(ARP_LINK *head)
功	能:	释放ARP链表
参	数:	ARP_LINK *head ARP链表头
返回值: 无
*******************************************************************/
extern void free_arp_link(ARP_LINK *arp_head);
#endif
