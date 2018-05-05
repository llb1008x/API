#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>				//struct ifreq
#include <sys/ioctl.h>			//ioctl、SIOCGIFADDR
#include <sys/socket.h>
#include <netinet/ether.h>		//ETH_P_ALL
#include <netpacket/packet.h>	//struct sockaddr_ll
#include <pthread.h>
#include <netinet/in.h>
void *send_arp_ask(void *arg);
int main(int argc,char *argv[])
{
	//1.创建通信用的原始套接字
	int sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	
	//2.创建发送线程
	pthread_t tid;
	pthread_create(&tid, NULL, (void *)send_arp_ask, (void *)sock_raw_fd);
	
	while(1)
	{
		printf("LINE:%d\n",__LINE__);
		//3.接收对方的ARP应答
		unsigned char recv_msg[1024] = "";
		recvfrom(sock_raw_fd, recv_msg, sizeof(recv_msg), 0, NULL, NULL);
		if(recv_msg[21] == 2)			//ARP应答
		{
			char resp_mac[18] = "";		//arp响应的MAC
			char resp_ip[16] = "";		//arp响应的IP
			
			sprintf(resp_mac, "%02x:%02x:%02x:%02x:%02x:%02x", \
			recv_msg[22],recv_msg[23],recv_msg[24],recv_msg[25],recv_msg[26],recv_msg[27]);
			sprintf(resp_ip, "%d.%d.%d.%d", recv_msg[28], recv_msg[29], recv_msg[30], recv_msg[31]);
			printf("IP:%s - MAC:%s\n",resp_ip, resp_mac);
		}
	}
	
	return 0;
}

void *send_arp_ask(void *arg)
{
	int i = 0;
	int sock_raw_fd = (int)arg;
	//1.根据各种协议首部格式构建发送数据报
	unsigned char send_msg[1024] = {
		//--------------组MAC--------14------
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //dst_mac: FF:FF:FF:FF:FF:FF
		0x00, 0x0c, 0x29, 0x75, 0xa6, 0x51, //src_mac: 00:0c:29:75:a6:51
		0x08, 0x06,							//类型：0x0806 ARP协议
		
		//--------------组ARP--------28-----
		0x00, 0x01, 0x08, 0x00,				//硬件类型1(以太网地址),协议类型0x0800(IP)	
		0x06, 0x04, 0x00, 0x01,				//硬件、协议地址分别是6、4，op:(1：arp请求，2：arp应答)
		0x00, 0x0c, 0x29, 0x75, 0xa6, 0x51,	//发送端的MAC地址
		172,  20,   226,  12,  				//发送端的IP地址
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//目的MAC地址（由于要获取对方的MAC,所以目的MAC置零）
		172,  20,   226,  11				//目的IP地址
	};
	
	//2.数据初始化
	struct sockaddr_ll sll;					//原始套接字地址结构
	struct ifreq ethreq;					//网络接口地址
	strncpy(ethreq.ifr_name, "eth0", IFNAMSIZ);	//指定网卡名称
	
	//3.将网络接口赋值给原始套接字地址结构
	ioctl(sock_raw_fd, SIOCGIFINDEX, (char *)&ethreq);
	bzero(&sll, sizeof(sll));
	sll.sll_ifindex = ethreq.ifr_ifindex;
	
	//4.本地机的IP
	if(!(ioctl(sock_raw_fd, SIOCGIFADDR, (char *)&ethreq)))	
	{
		int num = ntohl(((struct sockaddr_in*) (&ethreq.ifr_addr))->sin_addr.s_addr);
		for(i=0; i<4; i++)
		{
			send_msg[31-i] = num>>8*i & 0xff;	//将发送端的IP地址组包
		}
	}
	
	//5.获取本地机(eth0)的MAC
	if (!(ioctl(sock_raw_fd, SIOCGIFHWADDR, (char *) &ethreq)))
    {
		for(i=0; i<6; i++)
		{
			//将src_mac、发送端的MAC地址组包
			send_msg[22+i] = send_msg[6+i] = (unsigned char) ethreq.ifr_hwaddr.sa_data[i];			
		}
    }
	
	while(1)
	{
		int i = 0;
		int num[4] = {0};
		unsigned char input_buf[1024] = "";
		
		//6.获取所要扫描的网段（172.20.226.0）
		printf("input_dst_Network:172.20.226.0\n");
		fgets(input_buf, sizeof(input_buf), stdin);
		sscanf(input_buf, "%d.%d.%d.", &num[0], &num[1], &num[2]//目的IP地址 
		);
		
		//7.将键盘输入的信息组包
		for(i=0;i<4;i++)
			send_msg[38+i] = num[i];//将目的IP地址组包
		
		//8.给1~254的IP发送ARP请求
		for(i=1; i<255; i++)
		{
			send_msg[41] = i;
			int len = sendto(sock_raw_fd, send_msg, 42, 0 , (struct sockaddr *)&sll, sizeof(sll));
			if(len == -1)
			{
				perror("sendto");
			}
		}
		sleep(1);
	}
	return;
}
