#ifndef  IP_LINK_H
#define IP_LINK_H

typedef struct ip_link
{
	unsigned char ip[4];	//ip地址
	unsigned char mac[20];	//mac地址
	
	struct ip_link *next;	//下一节点
}IP_LINK;

extern IP_LINK *ip_head;

/***********************************************
函数功能:	把链表数据写入文件
参    数:	无
返 回 值:	无
***********************************************/
extern void write_ip_link();

/*********************************************
函数功能:	从文件中读取链表数据
参    数:	无
返 回 值:	无
*********************************************/
extern void read_ip_link();

/******************************************************************
函数功能:	插入ip到过滤链表
参    数:	IP_LINK *head ip过滤链表头 ，IP_LINK* p 待插入节点
返 回 值:	链表首地址
*******************************************************************/
extern IP_LINK *insert_ip_link(IP_LINK *head, IP_LINK* p);

/******************************************************************
函数功能:	插入mac到过滤链表
参    数:	IP_LINK *head ip过滤链表头 ，IP_LINK* p 待插入节点
返 回 值:	链表首地址
*******************************************************************/
extern IP_LINK *insert_mac_link(IP_LINK *head, IP_LINK* p);

/******************************************************************
函数功能:	查找ip
参    数:	IP_LINK *head ip过滤链表头 ，待查找的ip
返 回 值:	找到的节点地址
*******************************************************************/
extern IP_LINK *find_ip(IP_LINK *head, unsigned char *ip);

/******************************************************************
函数功能:	查找mac
参    数:	IP_LINK *head ip过滤链表头 ，待查找的mac地址
返 回 值:	找到的节点地址
*******************************************************************/
extern IP_LINK *find_mac(IP_LINK *head, unsigned char *mac);

/******************************************************************
函数功能:	打印ip过滤链表信息
参    数:	IP_LINK *head ip过滤链表头
返 回 值:	无
*******************************************************************/
extern void print_ip(IP_LINK *head);

/******************************************************************
函数功能:	打印mac过滤链表信息
参    数:	IP_LINK *head ip过滤链表头
返 回 值:	无
*******************************************************************/
extern void print_mac(IP_LINK *head);

/******************************************************************
函数功能:	删除ip过滤链表节点
参    数:	IP_LINK *head ip 过滤链表头，unsigned char *ip 待删除IP
返 回 值:	IP_LINK *head ip 过滤链表头
*******************************************************************/
extern IP_LINK *delete_ip(IP_LINK *head, unsigned char *ip);

/******************************************************************
函数功能:	删除mac过滤链表节点
参    数:	IP_LINK *head ip 过滤链表头，unsigned char *mac 待删除mac
返 回 值:	IP_LINK *head ip 过滤链表头
*******************************************************************/
extern IP_LINK *delete_mac(IP_LINK *head, unsigned char *mac);

/******************************************************************
函数功能:	释放ip 过滤链表
参    数:	IP_LINK *head ip过滤链表头
返 回 值:	无
*******************************************************************/
extern void free_ip_link(IP_LINK *head);

#endif
