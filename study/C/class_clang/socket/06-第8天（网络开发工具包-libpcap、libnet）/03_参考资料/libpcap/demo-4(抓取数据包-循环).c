#include <stdio.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <time.h>
#define BUFSIZE 1514

struct ether_header
{
	unsigned char ether_dhost[6];	//目的mac
	unsigned char ether_shost[6];	//源mac
	unsigned short ether_type;		//以太网类型
};
void ethernet_protocol_callback(unsigned char *argument,const struct pcap_pkthdr *packet_heaher,const unsigned char *packet_content)
{
	unsigned char *mac_string;				//
	struct ether_header *ethernet_protocol;
	unsigned short ethernet_type;			//以太网类型
	printf("---------------------*******------------------\n");
	ethernet_protocol = (struct ether_header *)packet_content;
	ethernet_type = ntohs(ethernet_protocol->ether_type);//获得以太网的类型
	printf("Ethernet type is :%04x\n",ethernet_type);
	switch(ethernet_type)
	{
		case 0x0800:printf("@@@@@@@The network layer is IP protocol\n");break;//ip
		case 0x0806:printf("@@@@@@@The network layer is ARP protocol\n");break;//arp
		case 0x0835:printf("The network layer is RARP protocol\n");break;//rarp
		default:break;
	}
	mac_string = (unsigned char *)ethernet_protocol->ether_shost;//获取源mac地址
	printf("Mac Source Address is %02x:%02x:%02x:%02x:%02x:%02x\n",*(mac_string+0),*(mac_string+1),*(mac_string+2),*(mac_string+3),*(mac_string+4),*(mac_string+5));
	mac_string = (unsigned char *)ethernet_protocol->ether_dhost;//获取目的mac
	printf("Mac Destination Address is %02x:%02x:%02x:%02x:%02x:%02x\n",*(mac_string+0),*(mac_string+1),*(mac_string+2),*(mac_string+3),*(mac_string+4),*(mac_string+5));
	usleep(800*1000);
}
int main()
{
	char error_content[PCAP_ERRBUF_SIZE];	//出错信息
	pcap_t * pcap_handle;
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
	pcap_loop(pcap_handle,-1,ethernet_protocol_callback,NULL);
	pcap_close(pcap_handle);
	return 0;
}