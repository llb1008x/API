#include "main.h"
#include "arp_link.h"
#include "ip_link.h"
#include "types.h"
#include <stdio.h>
#include <net/if.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include "get_interface.h"
#include <netinet/ether.h>
#include <netpacket/packet.h>

/******************************************************************
功	能:	ARP数据包
*******************************************************************/
unsigned char send_buf[42]={
	/*============MAC============*/
	0xff,0xff,0xff,0xff,0xff,0xff,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0x08,0x06,
	/*============ARP============*/
	0x00,0x01,0x08,0x00,
	0x06,0x04,0x00,0x01,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0,0,0,0,
	0x00,0x00,0x00,0x00,0x00,0x00,
	0,0,0,0
};

/******************************************************************
函	数:	int send_msg(int network_num,unsigned char *data_pkg,unsigned long data_len)
功	能:	根据网卡编号将对应的数据转发出去
参	数:	int network_num 网卡标号
		unsigned char *data_pkg 转发的数据
		unsigned long data_len 数据的长度
返回值: 转发数据的长度
*******************************************************************/
int send_pkg(int network_num,unsigned char *data_pkg,unsigned long data_len)
{
	struct ifreq ethreq;
	strncpy(ethreq.ifr_name,net_interface[network_num].name,IFNAMSIZ);
	ioctl(sock_raw_fd,SIOGIFINDEX,&ethreq);
	struct sockaddr_ll sll;
	bzero(&sll,sizeof(sll));
	sll.sll_ifindex=ethreq.ifr_ifindex;
	int len=sendto(sock_raw_fd,data_pkg,data_len,0,(struct sockaddr *)&sll,sizeof(sll));
	return len;
}

/******************************************************************
函	数:	int find_network_segment(unsigned char *ip)
功	能:	根据目的IP查找对应网段
参	数:	unsigned char *ip 目的IP
返回值: 网段编号
*******************************************************************/
int find_network_segment(unsigned char *ip)
{
	int i=0,j=get_interface_num();
	for(i=0;i<j;i++)
	{
		if((ip[0]&net_interface[i].netmask[0])==(net_interface[i].ip[0]&net_interface[i].netmask[0]) 
			&&(ip[1]&net_interface[i].netmask[1])==(net_interface[i].ip[1]&net_interface[i].netmask[1])
			&&(ip[2]&net_interface[i].netmask[2])==(net_interface[i].ip[2]&net_interface[i].netmask[2])
			&&(ip[3]&net_interface[i].netmask[3])==(net_interface[i].ip[3]&net_interface[i].netmask[3]))
		break;
	}
	return i;
}

/******************************************************************
函	数:	void *pthread_ip(void *arg)
功	能:	IP处理线程
		根据对应的网段选择发送的网卡
		没有目的IP的MAC地址则发送ARP请求
参	数:	void *arg 待转发的数据
返回值: 无
*******************************************************************/
void *pthread_ip(void *arg)
{
	unsigned char recv_buf[SIZE]="";		/*原始套接字数据包*/
	
	while(1)
	{	
		bzero(&recv_buf,sizeof(recv_buf));
		int len=recvfrom(sock_raw_fd,recv_buf,sizeof(recv_buf),0,NULL,NULL);
		
		if(recv_buf[12]==0x08 && recv_buf[13]==0x06)	/*接收ARP*/
		{	
			ARP_CACHE_LIST *arp_link=(ARP_CACHE_LIST *)malloc(sizeof(ARP_CACHE_LIST));	/*申请空间*/
			memcpy(arp_link->mac,recv_buf+6,6);
			memcpy(arp_link->ip,recv_buf+28,4);
			
			/*保存源MAC，源IP到ARP缓存表*/
			if(link_search_ip(arp_head,arp_link->ip)==NULL)
			{
				link_creat_head(&arp_head,arp_link);
			}
			
			//link_print(arp_head);
		}
		else if(recv_buf[12]==0x08 && recv_buf[13]==0x00)	/*接收ICMP请求*/
		{	
			unsigned char dst_ip[4]="";;
			memcpy(dst_ip,recv_buf+30,4);
			
			IP_FILTER_LIST *ip_pb=find_ip(ip_head,dst_ip);	/*目的ip过滤*/
			if(ip_pb!=NULL)
			{
				continue;
			}
			
			int network_num=find_network_segment(dst_ip);
			
			if(link_search_ip(arp_head,dst_ip)==NULL)
			{
				memcpy(send_buf+6,net_interface[network_num].mac,6);
				memcpy(send_buf+22,net_interface[network_num].mac,6);
				memcpy(send_buf+28,net_interface[network_num].ip,4);
				memcpy(send_buf+38,dst_ip,4);
				
				send_pkg(network_num,send_buf,42);	/*路由向目的IP发送ARP请求来获取目的IP的MAC地址*/
			}
			else	/*转发数据*/
			{
				ARP_CACHE_LIST *arp_link=link_search_ip(arp_head,dst_ip);
				memcpy(recv_buf,arp_link->mac,6);
				memcpy(recv_buf+6,net_interface[network_num].mac,6);
				
				send_pkg(network_num,recv_buf,len);
			}
		}
	}
}