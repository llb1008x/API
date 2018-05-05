#include <stdio.h>
#include <pcap.h>
#include <arpa/inet.h>
//pcap_lookupdev 获取网络接口
//pcap_lookupnet 获取ip、mask
//pcap_open_live 打开一个网络接口进行数据包获取,***在此函的参数中需要设置混杂模式
//pcap_next		 捕获一个数据包
//pcap_loop		 捕获多个数据包
//pcap_setfilter 设置BPF过滤规则
//pcap_compile	 编译过滤规则
//pcap_close	关闭libcap操作
int main()
{
	struct in_addr net_ip_address;
	struct in_addr net_mask_address;
	char *net_ip_string;
	char *net_mask_string;

	char error_content[PCAP_ERRBUF_SIZE];	//出错信息
	char *net_interface;					//接口名字
	unsigned int net_ip;					//网络地址
	unsigned int net_mask;					//子网掩码
	
	//获取网络接口
	net_interface = pcap_lookupdev(error_content);
	pcap_lookupnet(net_interface,&net_ip,&net_mask,error_content);
	
	printf("net_interface=%s\n",net_interface);
	
	net_ip_address.s_addr = net_ip;
	net_ip_string = inet_ntoa(net_ip_address);
	printf("ip=%s\n",net_ip_string);
	
	net_mask_address.s_addr = net_mask;
	net_mask_string = inet_ntoa(net_mask_address);
	printf("mask=%s\n",net_mask_string);
	
	
	return 0;
}