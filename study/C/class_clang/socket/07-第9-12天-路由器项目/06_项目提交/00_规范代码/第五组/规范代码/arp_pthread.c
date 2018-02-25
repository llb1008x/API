#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <netpacket/packet.h>
#include "arp_pthread.h"
#include "arp_link.h"
#include "get_interface.h"
#include "ip_link.h"

void *arp_pthread(void *arg)
{
	int sock_fd = (int )arg;
	unsigned char recv_buff[2048] = "";
	int num = get_interface_num();
	while(1)
	{
		bzero(recv_buff,sizeof(recv_buff));
		int ret = recvfrom(sock_fd, recv_buff, sizeof(recv_buff), 0, NULL, NULL);
		//只处理arp包的接收处理
		if((recv_buff[12]==0x08)&&(recv_buff[13]==0x06))    //ARP协议包
		{
				ARP_LINK *p = (ARP_LINK *)malloc(sizeof(ARP_LINK));
				memcpy(p->mac, recv_buff+6, 6);//A8的MAC
				memcpy(p->ip , recv_buff+28, 4);//A8的IP
				
				 inner_arp_link(&arp_head,p);
				 // printf_arp_link(arp_head);
				 
		}
		
		if((recv_buff[12]==0x08)&&(recv_buff[13]==0x00))    //IP协议包
		{
			
			ARP_LINK *p = link_search_ip(arp_head,recv_buff+30);
			//2获取目的地的ip和mac到arp缓存表里面
			if(p == NULL)//在arp表里未找到目的mac,就以广播方式发送arp
			{

				 unsigned char  send_buff[42]={
				0xff,0xff,0xff,0xff,0xff,0xff,
				0,0,0,0,0,0,
				0x08,0x06,
				0x00,0x01,0x08,0x00,
				0x06,0x04,0x00,0x01,
				0,0,0,0,0,0,
				0,0,0,0,
				0x00,0x00,0x00,0x00,0x00,0x00,
				0,0,0,0
				};
				int i=0;
				
				for(i=0;i<num;i++)
				{
					if(memcmp(recv_buff+30,net_interface[i].ip,3)==0)	
					{
						memcpy(send_buff+6,net_interface[i].mac,6);
						memcpy(send_buff+22,net_interface[i].mac,6);
						memcpy(send_buff+28,net_interface[i].ip,4);
						memcpy(send_buff+38,recv_buff+30,4);
						struct ifreq ethreq;
						strncpy(ethreq.ifr_name,net_interface[i].name,IFNAMSIZ);
						ioctl(sock_fd,SIOCGIFINDEX,&ethreq);
						struct sockaddr_ll sll;
						bzero(&sll,sizeof(sll));
						sll.sll_ifindex = ethreq.ifr_ifindex;	
						sendto(sock_fd,send_buff,42,0,(struct sockaddr *)&sll,sizeof(sll));
					}
				}
					
			}
			//转发数据icmp包
			else if(p != NULL)//在表里哪呢过找到mac
			{
				int i=0;
				for(i=0;i<num;i++)
				{
					if(memcmp(recv_buff+30,net_interface[i].ip,3)==0)	
					{
						memcpy(recv_buff+6,net_interface[i].mac,6);
						memcpy(recv_buff,p ->mac,6);
						struct ifreq ethreq;
						strncpy(ethreq.ifr_name,net_interface[i].name,IFNAMSIZ);
						ioctl(sock_fd,SIOCGIFINDEX,&ethreq);
						struct sockaddr_ll sll;
						bzero(&sll,sizeof(sll));
						sll.sll_ifindex = ethreq.ifr_ifindex;

					IP_LINK *ip_pb = find_ip(ip_head, recv_buff+30);//过滤
					if(ip_pb == NULL)	
					 sendto(sock_fd,recv_buff,ret,0,(struct sockaddr *)&sll,sizeof(sll));
						
					}
				}

			}
		
		}	
		
	}
	return NULL;
}
