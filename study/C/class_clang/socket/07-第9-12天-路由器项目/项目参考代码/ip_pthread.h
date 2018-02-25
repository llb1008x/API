#ifndef IP_PTHREAD_H
#define IP_PTHREAD_H
/******************************************************************
函	数:	void *ip_pthread(void *arg)
功	能:	IP处理线程
		根据对应的网段选择发送的网卡
		没有目的IP的MAC地址则发送ARP请求
参	数:	void *arg 待转发的数据
返回值: 无
*******************************************************************/
extern void *ip_pthread(void *arg);

#endif