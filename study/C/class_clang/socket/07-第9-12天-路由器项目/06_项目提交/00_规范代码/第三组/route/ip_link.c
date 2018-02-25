#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "ip_link.h"


//IP过滤链表首地址
IP *ip_head=NULL;

/************************************************************************
函	数:	ARP *ip_link_creat(IP *p_head, IP *p_new)
功	能:	创建IP地址过滤链表
参	数:	p_head：IP过滤链表首地址, p_new：新节点的地址
返回值：
*************************************************************************/
IP *ip_link_creat(IP *p_head, IP *p_new)
{
	IP *p_mov = p_head;
	
	if( p_mov == NULL )       //第一个节点为空
	{
		p_head = p_new;       //将新的节点地址赋给链表首地址
		p_new->next = NULL;
	}
	else 
	{
		if(ip_search_ip(p_head, (unsigned char*)p_new) != NULL)
		{
			return p_head;
		}
		else
		{
			while((	ip_search_ip(p_head, (unsigned char*)p_new) == NULL) && (p_mov->next != NULL))       //当前节点的后向不为空就继续循环
			{
				p_mov = p_mov->next;         //向后移动到下一个节点
			}
			p_mov->next = p_new;             //将新节点地址赋给当前节点的后面
		}
	}
	return p_head;
}

/************************************************************************
函	数:	IP *ip_search_ip( IP *p_head,unsigned char *ip )
功	能:	查找IP地址过滤链表节点
参	数:	p_head：IP过滤链表首地址，ip：32位无符号整数IP地址的指针
返回值：找到返回节点指针，找不到返回NULL
*************************************************************************/
IP *ip_search_ip( IP *p_head, unsigned char *ip )
{
	IP *p_mov=p_head;
	while( p_mov != NULL )               //当前节点不为空就继续循环
	{
		if( *(unsigned int *)(p_mov->ip) == *(unsigned int*)ip )    //若果当前节点的IP等于要找的IP
			return p_mov;                                 	        //返回当前节点
		p_mov = p_mov->next;                                        //否则向后移动继续查找下一个节点
	}
	return NULL;                    //找不到返回NULL
}

/************************************************************************
函	数:	IP *ip_search_mac(IP *p_head,unsigned char *mac)
功	能:	查找IP地址过滤链表节点
参	数:	p_head：IP过滤链表首地址，mac：MAC地址的指针
返回值：查找到的节点指针，查不到返回NULL
*************************************************************************/
IP *ip_search_mac( IP *p_head,unsigned char *mac )
{
	IP *p_mov=p_head;
	while( p_mov != NULL )               //当前节点不为空就继续循环
	{
		if( *(unsigned int *)(p_mov->mac) == *(unsigned int *)mac )    //若果当前节点的IP等于要找的IP
			return p_mov;                                 	        //返回当前节点
		p_mov = p_mov->next;                                        //否则向后移动继续查找下一个节点
	}
	return NULL;                    //找不到返回NULL
}

/************************************************************************
函	数:	void ip_print(IP *p_head)
功	能:	遍历IP地址过滤链表函数
参	数:	p_head：ARP缓存表链表首地址的地址
返回值：无
*************************************************************************/
void ip_print( IP *p_head )
{
	IP *p_mov = p_head;
	printf("路由器防火墙被过滤信息：\n");
	while( p_mov != NULL )
	{	
		printf("IP:%d.%d.%d.%d\n",p_mov->ip[0],p_mov->ip[1],p_mov->ip[2],p_mov->ip[3]);
		printf("MAC:%s\n",p_mov->mac);
		p_mov = p_mov->next;
	}
}

/******************************************************************
函  数：void save_file( IP *p_head )
功	能: 保存文件
参	数:	IP过滤链表的首地址
返回值: 无
*******************************************************************/

void save_file( IP *p_head )
{
	FILE *fp;
	IP *p_mov = p_head;
	char ip_buf[17]="";
	char mac_buf[108]="";
	
	fp = fopen("file", "w");
	
	while( p_mov!=NULL )
	{
		bzero(ip_buf, sizeof(ip_buf));
		bzero(mac_buf, sizeof(mac_buf));
		inet_ntop(AF_INET, p_mov->ip, ip_buf, 17);
		memcpy(p_mov->mac, mac_buf, strlen(mac_buf));
		fputs(ip_buf, fp);
		fputs(mac_buf, fp);
		p_mov=p_mov->next;
	}
	fclose(fp);
}

/************************************************************************
函	数:	int ip_link_del_ip(IP *p_head,unsigned char *ip)
功	能:	链表节点删除函数
参	数:	p_head：IP过滤链表首地址，ip：待删除节点中的IP地址
返回值：无
*************************************************************************/
void  ip_link_del_ip( IP *p_head, unsigned char *ip )
{
	IP *pb,*pf;
	pb = pf = p_head;
	
	while(memcmp( pb->ip, ip, 4)!= 0 && pb->next !=NULL )         //没有找到并且不是最后一个节点
	{
		pf = pb;
		pb = pb->next;
	}
	if(0 == memcmp(pb->ip, ip, 4))    //找到要删除的节点
	{		
		if(pb == p_head)			  //如果是头结点
			p_head = pb->next;
		else
			pf->next = pb->next;      //如果不是头结点
		free(pb);
	}

	else							  //找不到要删除的结点
	{
		printf("can't find the IP\n");
	}
}

/************************************************************************
函	数:	int ip_link_del_mac(IP **p_head,unsigned char *ip)
功	能:	链表节点删除函数
参	数:	p_head：IP过滤链表首地址，ip：待删除节点中的IP地址
返回值：无
*************************************************************************/
void  ip_link_del_mac( IP **p_head, unsigned char *mac_buf )
{
	IP *pb,*pf;
	pb = pf = *p_head;
	while(pb)
	{
				
		if(memcmp(pb->mac, mac_buf, 19)==0)    //找到要删除的节点
		{
			break;
		}		
		pb=pb->next;
	}
	if(pb == *p_head)                     //如果是头结点
	{			
		*p_head = pb->next;
	}
	else
		pf->next = pb->next;             //如果不是头结点
	free(pb);

}

 
