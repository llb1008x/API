/*=========================================================================
  工程名称：	libnet-udp-fack
  组成文件：	libnet-udp-fack.c
  功能描述： 	libnet实现arp欺骗
  维护记录：	
				2013-06-01 v1.0		add by sunplusedu
=========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libnet.h>

int main(int argc, char *argv[])
{
	char send_msg[1000] = "";
	char err_buf[100] = "";
	libnet_t *lib_net = NULL;
	int lens = 0;
	libnet_ptag_t lib_t = 0;
	unsigned char src_mac[6] = {0x00,0x0c,0x29,0x1b,0x22,0x0a};//发送者网卡地址
	unsigned char dst_mac[6] = {0xc8,0x9c,0xdc,0xa9,0x52,0x3c};//接收者网卡地址
    char *src_ip_str = "172.20.226.3"; //源主机IP地址
    char *dst_ip_str = "172.20.226.2"; //目的主机IP地址
	unsigned long src_ip,dst_ip = 0;

	lens = sprintf(send_msg, "1_lbt4_5#131200#C89CDCB70F19#0#0#0#305b#5:%d:%s:%s:%d:%s", 123,"he.liang",
	"HE-LIANG",32,"lh");

 	lib_net = libnet_init(LIBNET_LINK_ADV, "eth0", err_buf);	//初始化
	if(NULL == lib_net)
	{
		perror("libnet_init");
		exit(-1);
	}

	src_ip = libnet_name2addr4(lib_net,src_ip_str,LIBNET_RESOLVE);	//将字符串类型的ip转换为顺序网络字节流
	dst_ip = libnet_name2addr4(lib_net,dst_ip_str,LIBNET_RESOLVE);

	lib_t = libnet_build_udp(	//构造udp数据包
								2425,
								2425,
								8+lens,
								0,
								send_msg,
								lens,
								lib_net,
								0
							);

	lib_t = libnet_build_ipv4(	//构造ip数据包
								20+8+lens,
								0,
								500,
								0,
								10,
								17,
								0,
								src_ip,
								dst_ip,
								NULL,
								0,
								lib_net,
								0
							);

	lib_t = libnet_build_ethernet(	//构造以太网数据包
									(u_int8_t *)dst_mac,
									(u_int8_t *)src_mac,
									ETHERTYPE_IP,
									NULL,
									0,
									lib_net,
									0
								);
	int res = 0;
	res = libnet_write(lib_net);	//发送数据包
	if(-1 == res)
	{
		perror("libnet_write");
		exit(-1);
	}

	libnet_destroy(lib_net);	//销毁资源
	
	printf("----ok-----\n");
	return 0;
 }


