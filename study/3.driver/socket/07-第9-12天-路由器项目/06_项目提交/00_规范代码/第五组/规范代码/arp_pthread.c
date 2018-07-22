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
		//ֻ����arp���Ľ��մ���
		if((recv_buff[12]==0x08)&&(recv_buff[13]==0x06))    //ARPЭ���
		{
				ARP_LINK *p = (ARP_LINK *)malloc(sizeof(ARP_LINK));
				memcpy(p->mac, recv_buff+6, 6);//A8��MAC
				memcpy(p->ip , recv_buff+28, 4);//A8��IP
				
				 inner_arp_link(&arp_head,p);
				 // printf_arp_link(arp_head);
				 
		}
		
		if((recv_buff[12]==0x08)&&(recv_buff[13]==0x00))    //IPЭ���
		{
			
			ARP_LINK *p = link_search_ip(arp_head,recv_buff+30);
			//2��ȡĿ�ĵص�ip��mac��arp���������
			if(p == NULL)//��arp����δ�ҵ�Ŀ��mac,���Թ㲥��ʽ����arp
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
			//ת������icmp��
			else if(p != NULL)//�ڱ������ع��ҵ�mac
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

					IP_LINK *ip_pb = find_ip(ip_head, recv_buff+30);//����
					if(ip_pb == NULL)	
					 sendto(sock_fd,recv_buff,ret,0,(struct sockaddr *)&sll,sizeof(sll));
						
					}
				}

			}
		
		}	
		
	}
	return NULL;
}
