#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arp_pthread.h"
#include "arp_link.h"

/******************************************************************
��	��:	void *arp_pthread(void *arg)
��	��:	ARP�������߳�
		��ԴMAC��ԴIP����
��	��:	��
*******************************************************************/
void *arp_pthread(void *arg){
	ARP_LINK *p= (ARP_LINK *)arg;
	arp_head = inner_arp_link(arp_head, p);//��ӵ�������
	//ARP��Ӧ--
	return NULL;
}