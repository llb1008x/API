#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>				//struct ifreq
#include <sys/ioctl.h>			//ioctl��SIOCGIFADDR
#include <sys/socket.h>
#include <netinet/ether.h>		//ETH_P_ALL
#include <netpacket/packet.h>	//struct sockaddr_ll
#include <pthread.h>
#include <netinet/in.h>
void *send_arp_ask(void *arg);
int main(int argc,char *argv[])
{
	//1.����ͨ���õ�ԭʼ�׽���
	int sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	
	//2.���������߳�
	pthread_t tid;
	pthread_create(&tid, NULL, (void *)send_arp_ask, (void *)sock_raw_fd);
	
	while(1)
	{
		printf("LINE:%d\n",__LINE__);
		//3.���նԷ���ARPӦ��
		unsigned char recv_msg[1024] = "";
		recvfrom(sock_raw_fd, recv_msg, sizeof(recv_msg), 0, NULL, NULL);
		if(recv_msg[21] == 2)			//ARPӦ��
		{
			char resp_mac[18] = "";		//arp��Ӧ��MAC
			char resp_ip[16] = "";		//arp��Ӧ��IP
			
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
	//1.���ݸ���Э���ײ���ʽ�����������ݱ�
	unsigned char send_msg[1024] = {
		//--------------��MAC--------14------
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //dst_mac: FF:FF:FF:FF:FF:FF
		0x00, 0x0c, 0x29, 0x75, 0xa6, 0x51, //src_mac: 00:0c:29:75:a6:51
		0x08, 0x06,							//���ͣ�0x0806 ARPЭ��
		
		//--------------��ARP--------28-----
		0x00, 0x01, 0x08, 0x00,				//Ӳ������1(��̫����ַ),Э������0x0800(IP)	
		0x06, 0x04, 0x00, 0x01,				//Ӳ����Э���ַ�ֱ���6��4��op:(1��arp����2��arpӦ��)
		0x00, 0x0c, 0x29, 0x75, 0xa6, 0x51,	//���Ͷ˵�MAC��ַ
		172,  20,   226,  12,  				//���Ͷ˵�IP��ַ
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	//Ŀ��MAC��ַ������Ҫ��ȡ�Է���MAC,����Ŀ��MAC���㣩
		172,  20,   226,  11				//Ŀ��IP��ַ
	};
	
	//2.���ݳ�ʼ��
	struct sockaddr_ll sll;					//ԭʼ�׽��ֵ�ַ�ṹ
	struct ifreq ethreq;					//����ӿڵ�ַ
	strncpy(ethreq.ifr_name, "eth0", IFNAMSIZ);	//ָ����������
	
	//3.������ӿڸ�ֵ��ԭʼ�׽��ֵ�ַ�ṹ
	ioctl(sock_raw_fd, SIOCGIFINDEX, (char *)&ethreq);
	bzero(&sll, sizeof(sll));
	sll.sll_ifindex = ethreq.ifr_ifindex;
	
	//4.���ػ���IP
	if(!(ioctl(sock_raw_fd, SIOCGIFADDR, (char *)&ethreq)))	
	{
		int num = ntohl(((struct sockaddr_in*) (&ethreq.ifr_addr))->sin_addr.s_addr);
		for(i=0; i<4; i++)
		{
			send_msg[31-i] = num>>8*i & 0xff;	//�����Ͷ˵�IP��ַ���
		}
	}
	
	//5.��ȡ���ػ�(eth0)��MAC
	if (!(ioctl(sock_raw_fd, SIOCGIFHWADDR, (char *) &ethreq)))
    {
		for(i=0; i<6; i++)
		{
			//��src_mac�����Ͷ˵�MAC��ַ���
			send_msg[22+i] = send_msg[6+i] = (unsigned char) ethreq.ifr_hwaddr.sa_data[i];			
		}
    }
	
	while(1)
	{
		int i = 0;
		int num[4] = {0};
		unsigned char input_buf[1024] = "";
		
		//6.��ȡ��Ҫɨ������Σ�172.20.226.0��
		printf("input_dst_Network:172.20.226.0\n");
		fgets(input_buf, sizeof(input_buf), stdin);
		sscanf(input_buf, "%d.%d.%d.", &num[0], &num[1], &num[2]//Ŀ��IP��ַ 
		);
		
		//7.�������������Ϣ���
		for(i=0;i<4;i++)
			send_msg[38+i] = num[i];//��Ŀ��IP��ַ���
		
		//8.��1~254��IP����ARP����
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
