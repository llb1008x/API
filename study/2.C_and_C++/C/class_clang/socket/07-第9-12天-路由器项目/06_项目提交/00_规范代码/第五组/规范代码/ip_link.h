#ifndef IP_LINK_H
#define IP_LINK_H

typedef struct ip_link{
	unsigned char ip[4];	//ip地址四个字节
	struct ip_link *next;
}IP_LINK;

extern IP_LINK *ip_head;
extern void printf_ip_link(IP_LINK *head);
extern IP_LINK *find_ip(IP_LINK *head,unsigned char *ip);
extern void set_ip();
extern void inner_ip_link(IP_LINK **head,IP_LINK *p);
extern void del_ip_link(IP_LINK *head);
extern void save_ip_link();
#endif