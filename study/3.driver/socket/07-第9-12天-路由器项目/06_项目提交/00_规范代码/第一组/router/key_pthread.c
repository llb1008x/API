#include <stdio.h>
#include <unistd.h>
#include "types.h"
#include "ip_link.h"
#include "arp_link.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>

/******************************************************************
函	数:	void quit(void)
功	能:	退出程序
参	数:	无
返回值: 无
*******************************************************************/
void quit(void)
{
	exit(0);
}

/******************************************************************
函	数:	void home_page(void)
功	能:	主页信息
参	数:	无
返回值: 无
*******************************************************************/
void home_page(void)
{
	printf("********************************\n");
	printf("*   1.quit:退出程序            *\n");
	printf("*   2.setip:添加过滤IP         *\n");
	printf("*   3.delip:删除过滤IP         *\n");
	printf("*   4.catip:查看过滤IP         *\n");
	printf("*   5.catarp:查看ARP缓存表     *\n");
	printf("*   6.upgrade:上传IP配置文档   *\n");
	printf("********************************\n");
}

/******************************************************************
函	数:	IP_FILTER_LIST *get_filter_ip(void)
功	能:	获取键盘输入的IP
参	数:	无
返回值: 无
*******************************************************************/
IP_FILTER_LIST *get_filter_ip(void)
{
	char buf[16]="";
	bzero(buf,sizeof(buf));
	unsigned int ip;
	scanf("%s",buf);
	inet_pton(AF_INET,buf,&ip);	//地址转换函数
	IP_FILTER_LIST *pb=(IP_FILTER_LIST *)malloc(sizeof(IP_FILTER_LIST));	
	memcpy(pb->ip,&ip,4);
	return pb;
}

/******************************************************************
函	数:	void setip(void)
功	能:	添加过滤IP
参	数:	无
返回值: 无
*******************************************************************/
void setip(void)
{
	IP_FILTER_LIST *pb=get_filter_ip();	
	if(find_ip(ip_head,pb->ip)==NULL)
	{
		ip_head=insert_ip_link(ip_head,pb);
		save_ip_link();
	}
	else
	{
		printf("此IP已存在IP过滤链表中\n");
	}
}

/******************************************************************
函	数:	void delip(void)
功	能:	删除过滤IP
参	数:	无
返回值: 无
*******************************************************************/
void delip(void)
{
	IP_FILTER_LIST *pb=get_filter_ip();	
	if(find_ip(ip_head,pb->ip)!=NULL)
	{
		ip_head=del_ip_for_link(ip_head,pb->ip);
		save_ip_link();
	}
	else
	{
		printf("此IP不存在IP过滤链表中\n");
	}
}

/******************************************************************
函	数:	void catip(void)
功	能:	查看过滤IP
参	数:	无
返回值: 无
*******************************************************************/
void catip(void)
{
	printf_ip_link(ip_head);
}

/******************************************************************
函	数:	void catarp(void)
功	能:	查看ARP缓存表
参	数:	无
返回值: 无
*******************************************************************/
void catarp(void)
{
	link_print(arp_head);
}

/******************************************************************
函	数:	void *pthread_key(void *arg)
功	能:	按键输入线程函数
参	数:	无
返回值: 无
*******************************************************************/
void *pthread_key(void *arg)
{
	system("clear");
	home_page();
	while(1)
	{
		int opt;
		scanf("%d",&opt);
		switch(opt)
		{
		  case 1:quit(); break; /*退出程序*/
		  case 2:setip(); break; /*添加过滤IP*/
		  case 3:delip(); break; /*删除过滤IP*/
		  case 4:catip(); break; /*查看过滤链表*/
		  case 5:catarp(); break; /*查看ARP缓存表*/
		  // case 6:upgrade(); break; /*上传ip配置文档*/
		  default:printf("请输入有效的选项\n");
		}
	}
}