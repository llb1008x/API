#include <stdio.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <time.h>
//pcap_lookupdev 获取网络接口
//pcap_lookupnet 获取ip、mask
//pcap_open_live 打开一个网络接口进行数据包获取,***在此函的参数中需要设置混杂模式
//pcap_next		 捕获一个数据包
//pcap_loop		 捕获多个数据包
//pcap_setfilter 设置BPF过滤规则
//pcap_compile	 编译过滤规则
//pcap_close	关闭libcap操作
struct ether_header
{
	unsigned char ether_dhost[6];	//目的mac
	unsigned char ether_shost[6];	//源mac
	unsigned short ether_type;		//以太网类型
};
#define BUFSIZE 1514
int main()
{
	char error_content[PCAP_ERRBUF_SIZE];	//出错信息
	pcap_t * pcap_handle;
	unsigned char *packet_content;
	unsigned char *mac_string;				//
	unsigned short ethernet_type;			//以太网类型
	unsigned int net_ip;					//网络地址
	unsigned int net_mask;					//子网掩码
	char *net_interface;					//接口名字
	struct pcap_pkthdr protocol_header;
	struct ether_header *ethernet_protocol;
	struct bpf_program bpf_filter;
	char bpf_filter_string[] = "ip";
	
	//获取网络接口
	net_interface = pcap_lookupdev(error_content);
	pcap_lookupnet(net_interface,&net_ip,&net_mask,error_content);
	
	pcap_handle = pcap_open_live(net_interface,BUFSIZE,1,0,error_content);//打开网络接口
	pcap_compile(pcap_handle,&bpf_filter,bpf_filter_string,0,net_ip);//编译BPF过滤规则
	pcap_setfilter(pcap_handle,&bpf_filter);//设置过滤规则
	
	if (pcap_datalink(pcap_handle) != DLT_EN10MB)
		return 0;
	packet_content = pcap_next(pcap_handle,&protocol_header);
	
	printf("---------------------*******------------------\n");
	printf("capture a Packet from net_interface :\n");
	printf("%s\n",net_interface);
	printf("Capture Time is :\n");
	printf("%s",ctime((const time_t *)&protocol_header.ts.tv_sec));
	printf("Packet Lenght is :\n");
	printf("%d\n",protocol_header.len);
	ethernet_protocol = (struct ether_header *)packet_content;
	printf("Ethernet type is :\n");
	ethernet_type = ntohs(ethernet_protocol->ether_type);//获得以太网的类型
	printf("%04x\n",ethernet_type);
	switch(ethernet_type)
	{
		case 0x0800:printf("The network layer is IP protocol\n");break;//ip
		case 0x0806:printf("The network layer is ARP protocol\n");break;//arp
		case 0x0835:printf("The network layer is RARP protocol\n");break;//rarp
		default:break;
	}
	printf("Mac Source Address is :\n");
	mac_string = (unsigned char *)ethernet_protocol->ether_shost;//获取源mac地址
	printf("%02x:%02x:%02x:%02x:%02x:%02x\n",*(mac_string+0),*(mac_string+1),*(mac_string+2),*(mac_string+3),*(mac_string+4),*(mac_string+5));
	printf("Mac Destination Address is :\n");
	mac_string = (unsigned char *)ethernet_protocol->ether_dhost;//获取目的mac
	printf("%02x:%02x:%02x:%02x:%02x:%02x\n",*(mac_string+0),*(mac_string+1),*(mac_string+2),*(mac_string+3),*(mac_string+4),*(mac_string+5));
	printf("---------------------*******------------------\n");
	pcap_close(pcap_handle);
	return 0;
}