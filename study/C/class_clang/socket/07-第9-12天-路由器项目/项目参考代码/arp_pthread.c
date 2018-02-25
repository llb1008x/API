#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arp_pthread.h"
#include "arp_link.h"

/******************************************************************
函	数:	void *arp_pthread(void *arg)
功	能:	ARP包处理线程
		将源MAC和源IP保存
参	数:	无
*******************************************************************/
void *arp_pthread(void *arg){
	ARP_LINK *p= (ARP_LINK *)arg;
	arp_head = inner_arp_link(arp_head, p);//添加到链表中
	//ARP回应--
	return NULL;
}