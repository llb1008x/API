#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>				//struct ifreq
#include <sys/socket.h>
#include <netinet/ether.h>		//ETH_P_ALL
#include <netpacket/packet.h>	//struct sockaddr_ll
#include <sys/ioctl.h>			//ioctl��SIOCGIFADDR
unsigned short checksum(unsigned short *buf, int nword);
int main(int argc, char *argv[])
{
	//1.����ͨ���õ�ԭʼ�׽���
	int sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	
	//2.���ݸ���Э���ײ���ʽ�����������ݱ�
	unsigned char send_msg[1024] = {
		//--------------��MAC--------14------
		0xc8, 0x9c, 0xdc, 0xb7, 0x0f, 0x19, //dst_mac: c8:9c:dc:b7:0f:19
		0x00, 0x0c, 0x29, 0x75, 0xa6, 0x51, //src_mac: 00:0c:29:75:a6:51
		0x08, 0x00,                         //���ͣ�0x0800 IPЭ��
		//--------------��IP---------20------
		0x45, 0x00, 0x00, 40,             	//�汾�ţ�4, �ײ����ȣ�20�ֽ�, TOS:0, --�ܳ���--��
		0x00, 0x00, 0x00, 0x00,				//16λ��ʶ��3λ��־��13λƬƫ�ƶ�����0
		0x80, 6,   0x00, 0x00,				//TTL��128��Э�飺TCP��6����--16λ�ײ�У���--
		172,  20,   226,  12,				//src_ip: 172.20.226.12
		172,  20,   226,  11,				//dst_ip: 172.20.226.11
		//--------------��TCP--------20------
		0x1f, 0x40, 0x1f, 0x40,             //src_port:0x1f40(8000), dst_port:0x1f40(8000)
		0x00, 0x00, 0x00, 0x01,             //���
		0x00, 0x00, 0x00, 0x00,				//ȷ�����
		0x50, 0x02, 0x17, 0x70,             //����ƫ���ײ����ȣ�����4λΪ5*4=20��
											//����λռ6λ��0x02���ǽ�SYNλ��1������0x1770(6000)
		0x00, 0x00, 0x00, 0x00				//У��ͣ�TCPα�ײ�12B+TCP�ײ�20B+TCP���ݲ���0B��(����2B)
		
	};
	
	//3.TCPαͷ��
	unsigned char pseudo_head[1024] = {
		//------------TCPαͷ��-------12--
		172,  20,   226,  12,				//src_ip: 172.20.226.12
		172,  20,   226,  11,				//dst_ip: 172.20.226.11
		0x00, 6,    0x00, 20,             	//0,6��TCP��,#16λTCP����20���ֽ�
	};
	
	//4.����tcpУ�����Ҫ�����ݱ� = tcpαͷ�� + tcp���ݱ�
	memcpy(pseudo_head+12, send_msg+34, 20);//����udpУ���ʱ��Ҫ����αͷ��
	
	//5.��tcp���ݽ���У��
	*((unsigned short *)&send_msg[50]) = htons(checksum((unsigned short *)pseudo_head,32/2));
	*((unsigned short *)&send_msg[24]) = htons(checksum((unsigned short *)(send_msg+14),20/2));
	
	
	//6.��������
	struct sockaddr_ll sll;				//ԭʼ�׽��ֵ�ַ�ṹ
	struct ifreq ethreq;				//����ӿڵ�ַ
	
	strncpy(ethreq.ifr_name, "eth0", IFNAMSIZ);		//ָ����������
	if(-1 == ioctl(sock_raw_fd, SIOCGIFINDEX, &ethreq))	//��ȡ����ӿ�
	{
		perror("ioctl");
		close(sock_raw_fd);
		exit(-1);
	}
	
	/*������ӿڸ�ֵ��ԭʼ�׽��ֵ�ַ�ṹ*/
	bzero(&sll, sizeof(sll));
	sll.sll_ifindex = ethreq.ifr_ifindex;
	int len = sendto(sock_raw_fd, send_msg, 54, 0 , (struct sockaddr *)&sll, sizeof(sll));
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

