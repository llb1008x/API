#include "get_interface.h"
#include "arp_link.h"
#include "ip_link.h"
#include "ip_pthread.h"

void *ip_pthread( void *arg )
{
    //创建原始套接字
	int sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(sock_raw_fd<0)
	{
		perror("socket");
		exit(-1);
	}
	
	int i=0;
	int len;
	unsigned char recv_msg[1024]="";
	unsigned char recv_ip[4]="";
	char recv_mac[30]="";
	
	//组包
	unsigned char msg[42]={
	//mac头部(14位)
	0xff,0xff,0xff,0xff,0xff,0xff,       //dst_mac地址
	0x00,0x00,0x00,0x00,0x00,0x00,       //src_mac地址
	0x08,0x06,                           //类型:arp
	//arp头部(28位)
	0x00,0x01,                           //硬件类型:1
	0x08,0x00,                           //协议类型:ip
	0x06,0x04,   						 //硬件类型:6,协议类型:4
	0x00,0x01,                           //op:1
	0x00,0x00,0x00,0x00,0x00,0x00,       //src_mac
	0,0,0,0,                        	 //src_ip
	0x00,0x00,0x00,0x00,0x00,0x00,       //dst_mac置零
	0,0,0,0                        	     //dst_ip
	};
	
	//接收数据
	ARP *pb,*arp_new;                            //定义ARP缓存表指针
	IP  *p,*q;
	struct ifreq ethreq;                        //网络接口地址
	int interface_num = get_interface_num();    //获取本地网卡数目
	struct sockaddr_ll sll;
	
	while(1)
	{
		bzero(recv_msg, sizeof(recv_msg));
		len = recvfrom(sock_raw_fd, recv_msg, sizeof(recv_msg), 0, NULL, NULL);     //接收数据
		
		bzero(recv_ip, sizeof(recv_ip));
		
		if(recv_msg[12]==0x08 && recv_msg[13]==0x06)     //判断是否为ARP数据包
		{  
			arp_new = (ARP*)malloc(sizeof(ARP));         //开辟一个ARP缓存表节点的空间
			memcpy(arp_new->ip, recv_msg+14+14, 4);      //将ARP包中的源IP地址存入ARP缓存表节点
			memcpy(arp_new->mac, recv_msg+6, 6);  	     //将ARP包中的源MAC地址存入ARP缓存表节点
			pb=arp_search_ip(arp_head, recv_ip);         //按IP查找ARP缓存表
			if(pb == NULL)
			{			
				arp_head = arp_link_creat(arp_head, arp_new);      //将ARP缓存表节点插入ARP缓存表
			}
		}
		else if(recv_msg[12]==0x08 && recv_msg[13]==0x00)     //判断是否为IP数据包
		{	
		    if( recv_msg[23] == 0x01)                         //判断是否为ICMP数据包
			{
				memcpy(recv_ip, recv_msg+14+16, 4);           //获取dst_ip	
				q = ip_search_ip(ip_head, recv_ip);		      //查找得到的IP地址在IP过滤表中是否存在
			    if(q != NULL)
				    continue;
					
				sprintf(recv_mac,"%02x:%02x:%02x:%02x:%02x:%02x",
						recv_msg[6],recv_msg[7],recv_msg[8],recv_msg[9],recv_msg[10],recv_msg[11]);
				p = ip_search_mac(ip_head, (unsigned char *)recv_mac);		      //查找得到的mac地址在IP过滤表中是否存在
			    if(p != NULL)
				    continue;
				
				for(i=0;i<interface_num;i++)
				{
					if(memcmp(net_interface[i].ip,recv_ip,3)== 0)
					{
						strncpy(ethreq.ifr_name, net_interface[i].name, 16);  //指定网卡名称
						if(-1 == ioctl(sock_raw_fd, SIOCGIFINDEX, &ethreq))
						{
							perror("ioctl");
							close(sock_raw_fd);
							exit(-1);
						}
						break;
					}
				}
			
				bzero(&sll, sizeof(sll));
				sll.sll_ifindex = ethreq.ifr_ifindex;       //给sll赋值
				
				pb=arp_search_ip(arp_head, recv_ip);        //按IP查找ARP缓存表
				if(pb == NULL)                              //判断ARP缓存表中是否存在目标IP
				{  
					
					memcpy(msg+6, net_interface[i].mac, 6);       //设置src_MAC地址
					memcpy(msg+14+8, net_interface[i].mac, 6);    //设置src_MAC地址
					memcpy(msg+14+14, net_interface[i].ip, 4);    //设置src_IP地址
					memcpy(msg+14+24, recv_ip, 4);                //设置dst_IP地址
				     
					sendto(sock_raw_fd, msg, 42, 0, (struct sockaddr*)&sll, sizeof(sll)); //发送ARP请求包
					continue;
				}
				memcpy(recv_msg+6, net_interface[i].mac, 6);   //将网卡的MAC地址拷贝至源MAC地址
				memcpy(recv_msg, pb->mac, 6);                   //将ARP缓存表中的MAC地址拷贝至目的MAC地址
				sendto(sock_raw_fd, recv_msg, len, 0, (struct sockaddr*)&sll, sizeof(sll)); //转发数据包
			}
		}
	}
	close(sock_raw_fd);
}