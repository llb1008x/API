#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>				//struct ifreq
#include <sys/ioctl.h>			//ioctl��SIOCGIFADDR
#include <sys/socket.h>
#include <netinet/ether.h>		//ETH_P_ALL
#include <netpacket/packet.h>	//struct sockaddr_ll


unsigned short checksum(unsigned short *buf, int nword);//У��ͺ���
int main(int argc, char *argv[])
{
	//1.����ͨ���õ�ԭʼ�׽���
	int sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	
	//2.���ݸ���Э���ײ���ʽ�����������ݱ�
	unsigned char send_msg[1024] = {
		//--------------��MAC--------14------
		0xc8, 0x9c, 0xdc, 0xa7, 0x13, 0x7c, //dst_mac: b8:88:e3:e1:10:e6
		0xc8, 0x9c, 0xdc, 0xb7, 0x0f, 0x19, //src_mac: c8:9c:dc:b7:0f:19
		0x08, 0x00,                         //���ͣ�0x0800 IPЭ��
		//--------------��IP---------20------
		0x45, 0x00, 0x00, 0x00,           //�汾�ţ�4, �ײ����ȣ�20�ֽ�, TOS:0, --�ܳ���--��
		0x00, 0x00, 0x00, 0x00,				//16λ��ʶ��3λ��־��13λƬƫ�ƶ�����0
		0x80, 17,   0x00, 0x00,				//TTL��128��Э�飺UDP��17����16λ�ײ�У���
		172,  20,   223,  4,				//src_ip: 172.20.226.11
		172,  20,   223,  4,				//dst_ip: 172.20.226.13
		//--------------��UDP--------8------
		0x09, 0x79, 0x09, 0x79,             //src_port:0x0979(2425), dst_port:0x0979(2425)
		0x00, 0x00, 0x00, 0x00,               //#--16λUDP����--30���ֽڡ�#16λУ���
	};
	
	int len = sprintf(send_msg+42, "1:%d:%s:%s:%d:%s", 123,"xx", "xx",32,"������");//78���ֽ�
	if(len % 2 == 1)//�ж�len�Ƿ�Ϊ����
	{
		len++;//�����������len��Ӧ�ü�1(��ΪUDP�����ݲ��������Ϊż����Ҫ��0�)
	}
	
	*((unsigned short *)&send_msg[16]) = htons(20+8+len);//IP�ܳ��� = 20 + 8 + len
	*((unsigned short *)&send_msg[14+20+4]) = htons(8+len);//udp�ܳ��� = 8 + len
	//3.UDPαͷ��
	unsigned char pseudo_head[1024] = {
		//------------UDPαͷ��--------12--
		172,  20,   223,  4,				//src_ip: 172.20.226.17
		172,  20,   223,  4,				//dst_ip: 172.20.226.11
		0x00, 17,   0x00, 0x00,             	//0,17,#--16λUDP����--20���ֽ�
	};
	
	*((unsigned short *)&pseudo_head[10]) = htons(8 + len);//Ϊͷ���е�udp���ȣ�����ʵudp������ͬһ��ֵ��
	//4.����udpУ�����Ҫ�����ݱ� = udpαͷ�� + udp���ݱ�
	memcpy(pseudo_head+12, send_msg+34, 8+len);//--����udpУ���ʱ��Ҫ����αͷ��--
	//5.��IP�ײ�����У��
	*((unsigned short *)&send_msg[24]) = htons(checksum((unsigned short *)(send_msg+14),20/2));
	//6.--��UDP���ݽ���У��--
	*((unsigned short *)&send_msg[40]) = htons(checksum((unsigned short *)pseudo_head,(12+8+len)/2));
	
	
	//6.��������
	struct sockaddr_ll sll;					//ԭʼ�׽��ֵ�ַ�ṹ
	struct ifreq ethreq;					//����ӿڵ�ַ
	
	strncpy(ethreq.ifr_name, "eth0", IFNAMSIZ);			//ָ����������
	if(-1 == ioctl(sock_raw_fd, SIOCGIFINDEX, &ethreq))	//��ȡ����ӿ�
	{
		perror("ioctl");
		close(sock_raw_fd);
		exit(-1);
	}
	
	/*������ӿڸ�ֵ��ԭʼ�׽��ֵ�ַ�ṹ*/
	bzero(&sll, sizeof(sll));
	sll.sll_ifindex = ethreq.ifr_ifindex;
	len = sendto(sock_raw_fd, send_msg, 14+20+8+len, 0 , (struct sockaddr *)&sll, sizeof(sll));
	if(len == -1)
	{
		perror("sendto");
	}
	return 0;
}

unsigned short checksum(unsigned short *buf, int nword)
{
	unsigned long sum;
	for(sum = 0; nword > 0; nword--)
	{
		sum += htons(*buf);
		buf++;
	}
	sum = (sum>>16) + (sum&0xffff);
	sum += (sum>>16);
	return ~sum;
}

