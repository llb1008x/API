#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/ioctl.h>//ioctl SIOCGIFINDEX
#include <netinet/in.h>//htons
#include <net/if.h>//struct ifreq
#include <netinet/ether.h>//ETH_P_ALL
#include <netpacket/packet.h>

#include "get_interface.h"
#include "recv_net_data.h"
#include "arp_link.h"
#include "ip_deal.h"

/******************************************************************
函数功能:	根据目的ip查找网卡,确定发送数据的网卡号
参    数:	目的IP
返 回 值: 	网卡号
*******************************************************************/
int find_network_num(unsigned char *ip)
{
	int i = 0;
	int eth_num = get_interface_num();//网卡接口数量

	for(i=0; i<eth_num; i++)
	{
		if(memcmp(net_interface[i].ip, ip, 3) == 0)//比对ip，确定网卡号
			break;
	}
	return i;
}

/******************************************************************
函数功能:	发送ARP请求包获取mac
参    数:	RECV_DATA *arg 待转发的数据
返 回 值:	无
*******************************************************************/
void send_arp_ask(RECV_DATA *arg)
{
	int send_arp_ask_num = 0;
	RECV_DATA *recv = arg;
	int network_num = find_network_num((recv->data)+30);//获取发送数据的网卡
	for(send_arp_ask_num=0; send_arp_ask_num<3; send_arp_ask_num++)
	{
		//组ARP请求包
		unsigned char arp_ask_buf[42] = {
			//mac包(14)
			0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //目的mac(广播)
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //源mac
			0x08, 0x06,							//类型：ARP(0x0806)
			//ARP包(28)
			0x00, 0x01, 0x08, 0x00,				//硬件类型：以太网地址(0x0001),协议类型：IP(0x0800)
			0x06, 0x04,							//硬件地址长度：6，协议地址长度：4
			0x00, 0x01,							//op：ARP请求(0x0001)、ARP应答(0x0002)
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //源mac
			0,    0,    0,    0,				//源ip
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //目的mac
			0,    0,    0,    0					//目的ip
			};
		
		//给ARP请求包赋值
		memcpy(arp_ask_buf+6, net_interface[network_num].mac, 6);	//源mac
		memcpy(arp_ask_buf+22, net_interface[network_num].mac, 6);	//源mac
		memcpy(arp_ask_buf+28, net_interface[network_num].ip, 4);	//源ip
		memcpy(arp_ask_buf+38, (recv->data)+30, 4);					//目的ip
		
		//发送ARP应答包
		//网络数据初始化
		struct ifreq ethreq;	//网络接口地址
		strncpy(ethreq.ifr_name, net_interface[network_num].name, IFNAMSIZ);//指定网卡名称
		//将网络接口赋值给原始套接字结构体
		if(-1 == ioctl(sockfd, SIOCGIFINDEX, (char *)&ethreq))
		{
			perror("ioctl");
			close(sockfd);
			exit(-1);
		}

		struct sockaddr_ll sll;	//原始套接字地址结构体
		bzero(&sll, sizeof(sll));
		sll.sll_ifindex = ethreq.ifr_ifindex;
		
		int len_send = sendto(sockfd, arp_ask_buf, 42, 0, (struct sockaddr *)&sll, sizeof(sll));//发送数据
		if(len_send < 0)
		{
			perror("sendto");
			exit(-1);
		}
	}
}

/******************************************************************
函数功能:	转发icmp包
参    数:	void *arg 待转发的数据
返 回 值:	无
*******************************************************************/
void transpond_icmp(RECV_DATA *arg)
{
	RECV_DATA *icmp_data = arg;
	int network_num = find_network_num((icmp_data->data)+30);	//获取发送数据的网卡
	ARP_LINK * pb = ip_find_mac(arp_head, (icmp_data->data)+30);//查找mac,获取目的mac

	/*转发ICMP数据包*/
	memcpy((icmp_data->data), pb->arp_mac, 6);						//修改源MAC
	memcpy((icmp_data->data)+6, net_interface[network_num].mac, 6);	//修改目的MAC

	struct ifreq ethreq;													//网络接口地址	
	strncpy(ethreq.ifr_name, net_interface[network_num].name, IFNAMSIZ);	//指定网卡名称	
	ioctl(sockfd, SIOCGIFINDEX, &ethreq);		//获取网络接口	

	struct sockaddr_ll sll;						//原始套接字地址结构	
	bzero(&sll,sizeof(sll));	
	sll.sll_ifindex = ethreq.ifr_ifindex;

	int data_len = sendto(sockfd, icmp_data->data, icmp_data->data_len, 0,(struct sockaddr*)&sll, sizeof(sll));//发送消息
	if(data_len < 0)
	{
		printf("icmp_data.data_len=%d\n",icmp_data->data_len);
		perror("send msg");
	}
}

