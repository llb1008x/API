/*=========================================================================
  工程名称：	02-libpcap-demo
  组成文件：	02-libpcap-demo(安排规则,接收一个数据包).c
  功能描述： 	捕获多个网络数据包，然后对其进行数据的解析分析
  维护记录：	
				2013-06-01 v1.0		add by sunplusedu
=========================================================================*/
#include <stdio.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

struct ether_header
{
	unsigned char ether_dhost[6];	//目的mac
	unsigned char ether_shost[6];	//源mac
	unsigned short ether_type;		//以太网类型
};
#define BUFSIZE 1514

/*=========================================================================
  函数名称：int main(int argc,char *argv[])  
  功能描述：捕获多个网络数据包，然后对数据包进行解析
  参数传递：无
  返回数据：无
  维护记录：
			2013-06-01 v1.0		add by sunplusedu
=========================================================================*/
int main(int argc,char *argv[])
{
	pcap_t * pcap_handle;
	int ret = 0;
	char error_content[512] = "";	// 出错信息
	const unsigned char *p_packet_content = NULL; // 保存接收到的数据包的起始地址
	unsigned char *p_mac_string = NULL;			// 保存mac的地址，临时变量
	unsigned short ethernet_type = 0;			// 以太网类型
	char *p_net_interface_name = "eth0";		// 接口名字
	struct pcap_pkthdr protocol_header;
	struct ether_header *ethernet_protocol = NULL;
	struct bpf_program bpf_filter;
	bpf_u_int32 netp = 0, maskp = 0;
	char *bpf_filter_string = "arp or ip";
	//打开网络接口
	
	p_net_interface_name = pcap_lookupdev(error_content);
	if(NULL == p_net_interface_name)
	{
		perror("pcap_lookupdev");
		exit(-1);
	}
	
	pcap_handle = pcap_open_live(p_net_interface_name,1024,1,0,error_content);
	
	//获得网络号和掩码
	ret = pcap_lookupnet(p_net_interface_name, &netp, &maskp, error_content);
	if(ret == -1)
	{
		perror("pcap_lookupnet");
		exit(-1);
	}

	//编译BPF过滤规则
	if(pcap_compile(pcap_handle,&bpf_filter,bpf_filter_string,0,maskp) < 0)
	{
		perror("pcap_compile");
	}
	
	//设置过滤规则
	if(pcap_setfilter(pcap_handle,&bpf_filter) < 0)
	{
		perror("pcap_setfilter");
	}
	
	while(1)
	{

		p_packet_content = pcap_next(pcap_handle,&protocol_header); 
		//p_packet_content  所捕获的数据包的地址
		
		printf("------------------------------------------------------------------------\n");
		printf("capture a Packet from p_net_interface_name :%s\n",p_net_interface_name);
		printf("Capture Time is :%s",ctime((const time_t *)&protocol_header.ts.tv_sec));
		printf("Packet Lenght is :%d\n",protocol_header.len);
		
		/*
		*分析以太网中的 源mac、目的mac
		*/
		ethernet_protocol = (struct ether_header *)p_packet_content;  //struct ether_header 以太网帧头部
		p_mac_string = (unsigned char *)ethernet_protocol->ether_shost;//获取源mac
		
		printf("Mac Source Address is %02x:%02x:%02x:%02x:%02x:%02x\n",*(p_mac_string+0),*(p_mac_string+1),*(p_mac_string+2),*(p_mac_string+3),*(p_mac_string+4),*(p_mac_string+5));
		p_mac_string = (unsigned char *)ethernet_protocol->ether_dhost;//获取目的mac
		printf("Mac Destination Address is %02x:%02x:%02x:%02x:%02x:%02x\n",*(p_mac_string+0),*(p_mac_string+1),*(p_mac_string+2),*(p_mac_string+3),*(p_mac_string+4),*(p_mac_string+5));

		/*
		*获得以太网的数据包的地址，然后分析出上层网络协议的类型
		*/
		ethernet_type = ntohs(ethernet_protocol->ether_type);
		printf("Ethernet type is :%04x\n",ethernet_type);
		switch(ethernet_type)
		{
			case 0x0800:printf("The network layer is IP protocol\n");break;//ip
			case 0x0806:printf("The network layer is ARP protocol\n");break;//arp
			case 0x0835:printf("The network layer is RARP protocol\n");break;//rarp
			default:printf("The network layer unknow!!!\n");break;
		}
	}
	pcap_close(pcap_handle);
	return 0;
}