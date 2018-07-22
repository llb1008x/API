#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arp_link.h"

ARP_LINK *arp_head=NULL;

/******************************************************************
��	��:	void free_arp_link(ARP_LINK *head)
��	��:	�ͷ�ARP����
��	��:	ARP_LINK *head ARP����ͷ
����ֵ: ��
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
��	��:	void printf_arp_link(ARP_LINK *head)
��	��:	��ӡARP����
��	��:	ARP_LINK *head ARP����ͷ
����ֵ: ��
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
��	��:	ARP_LINK *find_arp_from_ip(ARP_LINK *head, unsigned char *ip)
��	��:	����IP����MAC
��	��:	ARP_LINK *head ARP����ͷ  unsigned char *ip IP��ַ
����ֵ: ARP_LINK *���ҵ��Ľڵ�
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
��	��:	ARP_LINK *inner_arp_link(ARP_LINK *head,ARP_LINK* p)
��	��:	����ARP����
��	��:	ARP_LINK *head ARP����ͷ  unsigned char *ip IP��ַ
����ֵ: ARP_LINK *head ARP����ͷ
*******************************************************************/
ARP_LINK *inner_arp_link(ARP_LINK *head,ARP_LINK* p){
	ARP_LINK *pb = find_arp_from_ip(head, p->ip);//�����Ƿ��иü�¼
	if(pb==NULL){//δ���ҵ�����������ֱ�Ӳ����ͷ����
		p->next = head;
		head = p;
	}else{//���ҵ����޸�����
		memcpy(pb->mac,p->mac,6);//����mac��ַ
	}
	return head;
}




