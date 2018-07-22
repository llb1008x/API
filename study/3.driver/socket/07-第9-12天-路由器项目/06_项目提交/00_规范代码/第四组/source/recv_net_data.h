#ifndef RECV_NET_DATA_H
#define RECV_NET_DATA_H

#include "arp_link.h"

extern int sockfd;			//原始套接字
extern ARP_LINK *arp_head;	//arp链表结构体全局变量

typedef struct recv_data
{
	int data_len ;				//数据长度
	unsigned char data[1530];	//数据
}RECV_DATA;

/***************************************
函数功能:	创建原始套接字
参    数:	NULL
返 回 值:	NULL
****************************************/
extern void *recv_net_data();

#endif
