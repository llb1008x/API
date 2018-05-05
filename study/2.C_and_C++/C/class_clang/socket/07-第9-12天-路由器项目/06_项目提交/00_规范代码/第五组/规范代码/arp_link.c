#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arp_link.h"

void inner_arp_link(ARP_LINK **head,ARP_LINK *p)
{
	ARP_LINK *p_mov = *head;
	if(*head ==NULL)
	{	*head = p;
		p->next = NULL;
	}
	else
	{
		while(p_mov->next != NULL && memcmp(p->ip,p_mov->ip,4) != 0)
		{
			p_mov = p_mov->next;
		}
		if(memcmp(p->ip,p_mov->ip,4)  == 0)
		 	return ;
		if(p_mov->next == NULL)
		{
			 p_mov->next = p;
			p->next = NULL;
		}
		
	}
}
ARP_LINK *link_search_ip(ARP_LINK *head, unsigned char *ip)
{	
	ARP_LINK *pb = head;	
	while(pb  != NULL)
	{	
		if(memcmp(pb->ip,ip,4)==0)
		{		
			return pb;
		}	
			pb = pb->next;
	}	
	return NULL;
}

void printf_arp_link(ARP_LINK *head)
{	
	printf("\n\n---------arp_link_start----------\n");
	ARP_LINK *pb = head;	
	while(pb  !=  NULL)
	{	
		printf("%d.%d.%d.%d-->", pb->ip[0],pb->ip[1],pb->ip[2],pb->ip[3]);		
		printf("%02x:%02x:%02x:%02x:%02x:%02x\n",		
		pb->mac[0],pb->mac[1],pb->mac[2],pb->mac[3],pb->mac[4],pb->mac[5]);	
		pb = pb->next;	
	}	
	printf("-------------------------------\n");
}



