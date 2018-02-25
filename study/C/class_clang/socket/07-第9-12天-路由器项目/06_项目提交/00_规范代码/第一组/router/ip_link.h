#ifndef IP_LINK_H
#define IP_LINK_H

#include "types.h"
#define ip_config_name "ip_config"

/******************************************************************
函	数:	void save_ip_link(void)
功	能:	保存链表数据到配置文件
参	数:	无
返回值: 无
*******************************************************************/
extern void save_ip_link(void);

/******************************************************************
函	数:	void read_ip_config(void)
功	能:	读取配置文件数据到链表
参	数:	无
返回值: 无
*******************************************************************/
extern void read_ip_config(void);

/******************************************************************
函	数:	void free_ip_link(IP_FILTER_LIST *head)
功	能:	释放ip过滤链表
参	数:	IP_FILTER_LIST *head ip过滤链表头
返回值: 无
*******************************************************************/
extern void free_ip_link(IP_FILTER_LIST *head);

/******************************************************************
函	数:	void printf_ip_link(IP_FILTER_LIST *head)
功	能:	打印ip过滤链表
参	数:	IP_FILTER_LIST *head ip过滤链表头
返回值: 无
*******************************************************************/
extern void printf_ip_link(IP_FILTER_LIST *head);

/******************************************************************
函	数:	IP_FILTER_LIST *find_ip(IP_FILTER_LIST *head, unsigned char *ip)
功	能:	查找ip过滤链表
参	数:	IP_FILTER_LIST *head ip过滤链表头  unsigned char *ip 待查找IP
返回值: IP_FILTER_LIST *找到的节点
*******************************************************************/
extern IP_FILTER_LIST *find_ip(IP_FILTER_LIST *head, unsigned char *ip);

/******************************************************************
函	数:	IP_FILTER_LIST *insert_ip_link(IP_FILTER_LIST *head, unsigned char *ip)
功	能:	插入ip过滤链表
参	数:	IP_FILTER_LIST *head ip过滤链表头  IP_FILTER_LIST* p 待插入节点
返回值: IP_FILTER_LIST *找到的节点
*******************************************************************/
extern IP_FILTER_LIST *insert_ip_link(IP_FILTER_LIST *head,IP_FILTER_LIST* p);

/******************************************************************
函	数:	IP_FILTER_LIST *del_ip_for_link(IP_FILTER_LIST *head, unsigned char *ip)
功	能:	删除ip过滤链表节点
参	数:	IP_FILTER_LIST *head ip过滤链表头  unsigned char *ip 待删除IP
返回值: IP_FILTER_LIST *head ip过滤链表头
*******************************************************************/
extern IP_FILTER_LIST *del_ip_for_link(IP_FILTER_LIST *head, unsigned char *ip);

#endif