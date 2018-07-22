#ifndef PING_PTHREAD_H
#define PING_PTHREAD_H

#include "types.h"
#include "main.h"
#include "get_interface.h"

/******************************************************************
函	数:	int send_msg(int network_num,unsigned char *data_pkg,unsigned long data_len)
功	能:	根据网卡编号将对应的数据转发出去
参	数:	int network_num 网卡标号
		unsigned char *data_pkg 转发的数据
		unsigned long data_len 数据的长度
返回值: 转发数据的长度
*******************************************************************/
extern int send_pkg(int network_num,unsigned char *data_pkg,unsigned long data_len);

/******************************************************************
函	数:	int find_network_segment(unsigned char *ip)
功	能:	根据目的IP查找对应网段
参	数:	unsigned char *ip 目的IP
返回值: 网段编号
*******************************************************************/
extern int find_network_segment(unsigned char *ip);

/******************************************************************
函	数:	void *pthread_ip(void *arg)
功	能:	IP处理线程
		根据对应的网段选择发送的网卡
		没有目的IP的MAC地址则发送ARP请求
参	数:	void *arg 待转发的数据
返回值: 无
*******************************************************************/
extern void *pthread_ip(void *arg);

#endif