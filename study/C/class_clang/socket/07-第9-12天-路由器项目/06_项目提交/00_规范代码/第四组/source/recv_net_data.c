#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ether.h>

#include "recv_net_data.h"
#include "arp_link.h"
#include "ip_deal.h"
#include "ip_link.h"

ARP_LINK *arp_head = NULL;//arp链表结构体全局变量
int sockfd;//原始套接字

/***************************************
函数功能:	创建原始套接字
参    数:	NULL
返 回 值:	NULL
****************************************/
void *recv_net_data()
{
	unsigned char recv_buf[1530] = "";
	int len_recv = 0;
	
	//创建原始套接字
	sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(sockfd < 0)//创建失败
	{
		perror("socket");
		exit(-1);
	}
	
	while(1)
	{
		bzero(&recv_buf, sizeof(recv_buf));//清空数组内容
		len_recv = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, NULL, NULL);//接收网络数据包
		if(len_recv < 0)
		{
			perror("recvfrom");
			exit(-1);
		}
		
		//判断数据包类型
		if(recv_buf[12] == 0x08 && recv_buf[13] == 0x06)		 //ARP包
		{
			ARP_LINK *arp = (ARP_LINK *)malloc(sizeof(ARP_LINK));//申请节点
			
			memcpy(arp->arp_mac, recv_buf+6, 6);	//获取mac
			memcpy(arp->arp_ip, recv_buf+28, 4);	//获取ip

			insert_link(&arp_head, arp);			//插入ARP缓存链表
		}
		
		else if(recv_buf[12] == 0x08 && recv_buf[13] == 0x00)//IP包
		{
			RECV_DATA *recv = (RECV_DATA *)malloc(sizeof(RECV_DATA));
			recv->data_len = len_recv;
			memcpy(recv->data, recv_buf, len_recv);
			
			if(recv_buf[23] == 0x01)//ICMP包
			{
				//判断是否满足过滤条件
				unsigned char ip2[4] = "";
				unsigned char mac[6] = "";
				memcpy(ip2, recv_buf+30, 4);
				memcpy(mac, recv_buf+6, 6);	//获取mac
				char mac_p[20] = "";
				sprintf(mac_p, "%02x:%02x:%02x:%02x:%02x:%02x",
					mac[0],	mac[1],	mac[2],
					mac[3],	mac[4],	mac[5]);
				IP_LINK *ip_temp = find_ip(ip_head, ip2);		//查找ip
				IP_LINK *mac_temp = find_mac(ip_head, (unsigned char *)mac_p);	//查找mac
				if((ip_temp != NULL) || (mac_temp != NULL))		//若ip或者mac处于过滤链表，则不转发icmp包
				{
					continue;
				}
				else
				{
					/*判断目的ip  和mac  是否存入路由表*/
					unsigned char ipl[4] = "";
					memcpy(ipl, recv_buf+30,4);
					ARP_LINK *ip_pb = ip_find_mac(arp_head, ipl);//查看ARP表，ip是否存在
					if(ip_pb != NULL)		//若存在，则转发ICMP包
					{
						transpond_icmp(recv);//转发ICMP包
					}
					else if(ip_pb == NULL)	//不存在，则发送ARP请求包获取mac
					{
						send_arp_ask(recv);	//发送ARP请求包,获取mac
					}
				}
			}
		}
	}
	return NULL;
}
