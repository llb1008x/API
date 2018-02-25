#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arp_link.h"

ARP_LINK *arp_head=NULL;

/******************************************************************
函	数:	void free_arp_link(ARP_LINK *head)
功	能:	释放ARP链表
参	数:	ARP_LINK *head ARP链表头
返回值: 无
*******************************************************************/
void free_arp_link(ARP_LINK *head){
	ARP_LINK *pb = head;
	while(head){
		pb = head->next;
		free(head);
		head = pb;
	}
}

/******************************************************************
函	数:	void printf_arp_link(ARP_LINK *head)
功	能:	打印ARP链表
参	数:	ARP_LINK *head ARP链表头
返回值: 无
*******************************************************************/
void printf_arp_link(ARP_LINK *head){
	printf("\n\n---------arp_link_start----------\n");
	ARP_LINK *pb = head;
	while(pb){
		printf("%d.%d.%d.%d-->", pb->ip[0],pb->ip[1],pb->ip[2],pb->ip[3]);
		printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
				pb->mac[0],pb->mac[1],pb->mac[2],pb->mac[3],pb->mac[4],pb->mac[5]);
		pb = pb->next;
	}
	printf("---------arp_link_end----------\n\n");
}

/******************************************************************
函	数:	ARP_LINK *find_arp_from_ip(ARP_LINK *head, unsigned char *ip)
功	能:	根据IP查找MAC
参	数:	ARP_LINK *head ARP链表头  unsigned char *ip IP地址
返回值: ARP_LINK *查找到的节点
*******************************************************************/
ARP_LINK *find_arp_from_ip(ARP_LINK *head, unsigned char *ip){
	ARP_LINK *pb = head;
	while(pb){
		if(memcmp(pb->ip,ip,4)==0){
			break;
		}
		pb = pb->next;
	}
	return pb;
}

/******************************************************************
函	数:	ARP_LINK *inner_arp_link(ARP_LINK *head,ARP_LINK* p)
功	能:	插入ARP链表
参	数:	ARP_LINK *head ARP链表头  unsigned char *ip IP地址
返回值: ARP_LINK *head ARP链表头
*******************************************************************/
ARP_LINK *inner_arp_link(ARP_LINK *head,ARP_LINK* p){
	ARP_LINK *pb = find_arp_from_ip(head, p->ip);//查找是否有该记录
	if(pb==NULL){//未查找到，插入链表，直接插入表头方便
		p->next = head;
		head = p;
	}else{//查找到，修改链表
		memcpy(pb->mac,p->mac,6);//拷贝mac地址
	}
	return head;
}




