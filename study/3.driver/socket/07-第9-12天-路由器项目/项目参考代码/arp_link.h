#ifndef ARP_LINK_H
#define ARP_LINK_H

typedef struct arp_link{
	unsigned char ip[4];//ip��ַ�ĸ��ֽ�
	unsigned char mac[6];//mac��ַ�����ֽ�
	struct arp_link *next;
}ARP_LINK;
extern ARP_LINK *arp_head;

/******************************************************************
��	��:	ARP_LINK *inner_arp_link(ARP_LINK *head,ARP_LINK* p)
��	��:	����ARP����
��	��:	ARP_LINK *head ARP����ͷ  unsigned char *ip IP��ַ
����ֵ: ARP_LINK *head ARP����ͷ
*******************************************************************/
extern ARP_LINK *inner_arp_link(ARP_LINK *arp_head,ARP_LINK* p);

/******************************************************************
��	��:	void printf_arp_link(ARP_LINK *head)
��	��:	��ӡARP����
��	��:	ARP_LINK *head ARP����ͷ
����ֵ: ��
*******************************************************************/
extern void printf_arp_link(ARP_LINK *arp_head);

/******************************************************************
��	��:	ARP_LINK *find_arp_from_ip(ARP_LINK *head, unsigned char *ip)
��	��:	����IP����MAC
��	��:	ARP_LINK *head ARP����ͷ  unsigned char *ip IP��ַ
����ֵ: ARP_LINK *���ҵ��Ľڵ�
*******************************************************************/
extern ARP_LINK *find_arp_from_ip(ARP_LINK *arp_head, unsigned char *ip);
/******************************************************************
��	��:	void free_arp_link(ARP_LINK *head)
��	��:	�ͷ�ARP����
��	��:	ARP_LINK *head ARP����ͷ
����ֵ: ��
*******************************************************************/
extern void free_arp_link(ARP_LINK *arp_head);
#endif
