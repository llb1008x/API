#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>				//struct ifreq
#include <sys/socket.h>
#include <netinet/ether.h>		//ETH_P_ALL
#include <netpacket/packet.h>	//struct sockaddr_ll
#include <sys/ioctl.h>			//ioctl、SIOCGIFADDR
unsigned short checksum(unsigned short *buf, int nword);
int main(int argc, char *argv[])
{
	//1.创建通信用的原始套接字
	int sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	
	//2.根据各种协议首部格式构建发送数据报
	unsigned char send_msg[1024] = {
		//--------------组MAC--------14------
		0xc8, 0x9c, 0xdc, 0xb7, 0x0f, 0x19, //dst_mac: c8:9c:dc:b7:0f:19
		0x00, 0x0c, 0x29, 0x75, 0xa6, 0x51, //src_mac: 00:0c:29:75:a6:51
		0x08, 0x00,                         //类型：0x0800 IP协议
		//--------------组IP---------20------
		0x45, 0x00, 0x00, 40,             	//版本号：4, 首部长度：20字节, TOS:0, --总长度--：
		0x00, 0x00, 0x00, 0x00,				//16位标识、3位标志、13位片偏移都设置0
		0x80, 6,   0x00, 0x00,				//TTL：128、协议：TCP（6）、--16位首部校验和--
		172,  20,   226,  12,				//src_ip: 172.20.226.12
		172,  20,   226,  11,				//dst_ip: 172.20.226.11
		//--------------组TCP--------20------
		0x1f, 0x40, 0x1f, 0x40,             //src_port:0x1f40(8000), dst_port:0x1f40(8000)
		0x00, 0x00, 0x00, 0x01,             //序号
		0x00, 0x00, 0x00, 0x00,				//确认序号
		0x50, 0x02, 0x17, 0x70,             //数据偏（首部长度）移在4位为5*4=20，
											//保留位占6位，0x02就是将SYN位置1、窗口0x1770(6000)
		0x00, 0x00, 0x00, 0x00				//校验和（TCP伪首部12B+TCP首部20B+TCP数据部分0B）(靠左2B)
		
	};
	
	//3.TCP伪头部
	unsigned char pseudo_head[1024] = {
		//------------TCP伪头部-------12--
		172,  20,   226,  12,				//src_ip: 172.20.226.12
		172,  20,   226,  11,				//dst_ip: 172.20.226.11
		0x00, 6,    0x00, 20,             	//0,6（TCP）,#16位TCP长度20个字节
	};
	
	//4.构建tcp校验和需要的数据报 = tcp伪头部 + tcp数据报
	memcpy(pseudo_head+12, send_msg+34, 20);//计算udp校验和时需要加上伪头部
	
	//5.对tcp数据进行校验
	*((unsigned short *)&send_msg[50]) = htons(checksum((unsigned short *)pseudo_head,32/2));
	*((unsigned short *)&send_msg[24]) = htons(checksum((unsigned short *)(send_msg+14),20/2));
	
	
	//6.发送数据
	struct sockaddr_ll sll;				//原始套接字地址结构
	struct ifreq ethreq;				//网络接口地址
	
	strncpy(ethreq.ifr_name, "eth0", IFNAMSIZ);		//指定网卡名称
	if(-1 == ioctl(sock_raw_fd, SIOCGIFINDEX, &ethreq))	//获取网络接口
	{
		perror("ioctl");
		close(sock_raw_fd);
		exit(-1);
	}
	
	/*将网络接口赋值给原始套接字地址结构*/
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

