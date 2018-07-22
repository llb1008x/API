#include <stdio.h>
#include <pcap.h>
#include <arpa/inet.h>
int main()
{
	struct in_addr net_ip_address;
	struct in_addr net_mask_address;
	char *net_ip_string;
	char *net_mask_string;

	char error_content[PCAP_ERRBUF_SIZE];	//������Ϣ
	char *net_interface;					//�ӿ�����
	unsigned int net_ip;					//�����ַ
	unsigned int net_mask;					//��������
	
	//��ȡ����ӿ�
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