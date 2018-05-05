#ifndef __ARP_LINK_H__
#define __ARP_LINK_H__
typedef struct arp_link
{
	unsigned char ip[4];
	unsigned char mac[6];
	struct arp_link *next;
}ARP_LINK;
extern ARP_LINK *arp_head;
extern void inner_arp_link(ARP_LINK ** arp_head,ARP_LINK *p);
extern void printf_arp_link(ARP_LINK *head);
extern ARP_LINK *link_search_ip(ARP_LINK *head, unsigned char *ip);

#endif