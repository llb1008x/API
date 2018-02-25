#include <stdio.h>
#include <pcap.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#define BUFSIZE 1514
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
struct ether_header
{
	unsigned char ether_dhost[6];	//目的mac
	unsigned char ether_shost[6];	//源mac
	unsigned short ether_type;		//以太网类型
};
// struct in_addr
// {
	// unsigned int s_addr;			//存放ip地址
// };
struct arp_header
{
	unsigned short arp_hardware_type;//硬件地址类型
	unsigned short arp_protocol_type;//协议地址类型
	unsigned char arp_hardware_length;//硬件地址长度
	unsigned char arp_protocol_length;//协议地址长度
	unsigned short arp_operation_code;//操作类型
	unsigned char arp_source_ethernet_address[6];//源mac
	unsigned char arp_source_ip_address[4];//源ip
	unsigned char arp_destination_ethernet_address[6];//目的mac
	unsigned char arp_destination_ip_address[4];//目的ip
};
void arp_protocol_packet_callback(unsigned char *argument,const struct pcap_pkthdr *packet_heaher,const unsigned char *packet_content)
{
	struct arp_header *arp_protocol;
	ushort protocol_type;
	ushort hardware_type;
	ushort operation_code;
	uchar * mac_string;
	struct in_addr source_ip_address;
	struct in_addr destinationi_ip_address;
	uchar hardware_length;
	uchar protocol_length;

	printf("---------------------***arp****------------------\n");
	arp_protocol = (struct arp_header *)(packet_content+14);//得到arp数据包
	hardware_type = ntohs(arp_protocol->arp_hardware_type);
	protocol_type = ntohs(arp_protocol->arp_protocol_type);
	operation_code = ntohs(arp_protocol->arp_operation_code);
	hardware_length = arp_protocol->arp_hardware_length;
	protocol_length = arp_protocol->arp_protocol_length;
	printf("ARP hardware_type type:%d\n",hardware_type);
	printf("ARP protocol_type type:%d\n",protocol_type);
	printf("ARP operation_code type:%d\n",operation_code);
	printf("ARP hardware_length type:%d\n",hardware_length);
	printf("ARP protocol_length type:%d\n",protocol_length);
	switch(operation_code)
	{
		case 1:printf("ARP request protocol\n");break;
		case 2:printf("ARP reply protocol\n");break;
		case 3:printf("RARP request protocol\n");break;
		case 4:printf("RARP reply protocol\n");break;
		default:break;
	}
	mac_string = (unsigned char *)arp_protocol->arp_source_ethernet_address;//获取源mac地址
	printf("Mac Source Address is %02x:%02x:%02x:%02x:%02x:%02x\n",*(mac_string+0),*(mac_string+1),*(mac_string+2),*(mac_string+3),*(mac_string+4),*(mac_string+5));
	memcpy((void *)&source_ip_address,(void *)arp_protocol->arp_source_ip_address,sizeof(struct in_addr));
	printf("Source ip address is %s\n",inet_ntoa(source_ip_address));
	mac_string = (unsigned char *)arp_protocol->arp_destination_ethernet_address;//获取目的mac
	printf("Mac Destination Address is %02x:%02x:%02x:%02x:%02x:%02x\n",*(mac_string+0),*(mac_string+1),*(mac_string+2),*(mac_string+3),*(mac_string+4),*(mac_string+5));
	memcpy((void *)&destinationi_ip_address,(void *)arp_protocol->arp_destination_ip_address,sizeof(struct in_addr));
	printf("Destination ip address is %s\n",inet_ntoa(destinationi_ip_address));
}
void ethernet_protocol_callback(unsigned char *argument,const struct pcap_pkthdr *packet_header,const unsigned char *packet_content)
{
	unsigned char *mac_string;				//
	struct ether_header *ethernet_protocol;
	unsigned short ethernet_type;			//以太网类型
	// printf("---------------------***ethernet****------------------\n");
	ethernet_protocol = (struct ether_header *)packet_content;
	ethernet_type = ntohs(ethernet_protocol->ether_type);//获得以太网的类型
	// printf("Ethernet type is :%04x\n",ethernet_type);
	switch(ethernet_type)
	{
		// case 0x0800:printf("@@@@@@@The network layer is IP protocol\n");break;//ip
		case 0x0806:
					{
						arp_protocol_packet_callback(argument,packet_header,packet_content);	
					}break;//arp
		// case 0x0835:printf("The network layer is RARP protocol\n");break;//rarp
		default:break;
	}
	// usleep(800*1000);
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
	// char bpf_filter_string[] = "arp";
	
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