#ifndef TYPES_H
#define TYPES_H

typedef struct arp_cache_list
{
	unsigned char ip[4];		/* IP地址 */
	unsigned char mac[6];		/* MAC地址 */
	struct arp_cache_list *next;
}ARP_CACHE_LIST;

ARP_CACHE_LIST *arp_head;

typedef struct ip_filter_list
{
	unsigned char ip[4];		/* IP地址 */
	struct ip_filter_list *next;
}IP_FILTER_LIST;

IP_FILTER_LIST *ip_head;

#endif