#ifndef __KEY_PTHREAD_H__
#define __KEY_PTHREAD_H__

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include "arp_link.h"
#include "ip_link.h"

//声明打印帮助信息
extern void print_help(void);

//声明遍历ARP缓存表
extern void arp_print(ARP *p_head);

//声明路由器网卡信息
extern void ifconfig();

//添加IP地址
extern void filt_ip(char *ip_buf);

//添加MAC地址
extern void filt_mac(char *mac_buf);

//删除IP地址
extern void del_ip( char *ip_buf );

//删除mac地址
extern void del_mac( char *mac_buf );

//声明设置防火墙
extern void firewall();

//声明键盘控制的线程
extern void *key_pthread( void *arg);


#endif