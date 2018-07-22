#ifndef __IP_LINK_H__
#define __IP_LINK_H__


//声明IP地址过滤链表
typedef struct ip_link{
	unsigned char ip[4];		//IP地址
	unsigned char mac[19];		//MAC地址
	struct ip_link *next;		//链表后项地址
}IP;

//声明IP过滤链表
extern IP *ip_head;


/************************************************************************
函	数:	IP *ip_link_creat( IP *p_head, IP *p_new )
功	能:	创建IP地址过滤链表
参	数:	p_head：IP过滤链表首地址, p_new：新节点的地址
返回值：无
*************************************************************************/
extern IP *ip_link_creat( IP *p_head, IP *p_new );


/************************************************************************
函	数:	IP *ip_search_ip(IP *p_head,unsigned char *ip)
功	能:	查找IP地址过滤链表节点
参	数:	p_head：IP过滤链表首地址，ip：32位无符号整数IP地址的指针
返回值：查找到的节点指针，查不到返回NULL
*************************************************************************/
extern IP *ip_search_ip( IP *p_head,unsigned char *ip );


/************************************************************************
函	数:	IP *ip_search_mac(IP *p_head,unsigned char *mac)
功	能:	查找IP地址过滤链表节点
参	数:	p_head：IP过滤链表首地址，mac：MAC地址的指针
返回值：查找到的节点指针，查不到返回NULL
*************************************************************************/
extern IP *ip_search_mac( IP *p_head,unsigned char *mac );


/************************************************************************
函	数:	void ip_print(IP *p_head)
功	能:	遍历IP地址过滤链表函数
参	数:	p_head：IP过滤链表首地址
返回值：无
*************************************************************************/
extern void ip_print( IP *p_head );


/******************************************************************
函  数：void save_file( IP *p_head )
功	能: 保存文件
参	数:	IP过滤链表的首地址
返回值: 无
*******************************************************************/
extern void save_file( IP *p_head );


/************************************************************************
函	数:	int ip_link_del_ip( IP *p_head, unsigned char *ip )
功	能:	链表节点删除函数
参	数:	p_head：IP过滤链表首地址，ip：待删除节点中的IP地址
返回值：无
*************************************************************************/
extern void ip_link_del_ip( IP *p_head, unsigned char *ip );

/************************************************************************
函	数:	int ip_link_delete(IP **p_head,unsigned char *ip)
功	能:	链表节点删除函数
参	数:	p_head：IP过滤链表首地址，ip：待删除节点中的IP地址
返回值：无
*************************************************************************/
extern void ip_link_del_mac( IP **p_head, unsigned char *mac_buf );


#endif