#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "firewall_pthread.h"
#include "ip_link.h"
#include "arp_link.h"
#include "recv_net_data.h"
#include "get_interface.h"
#include "tftp_deal.h"

//路由器命令协议
//1.help:		打印帮助信息
//2.showarp：	查看arp缓存表
//3.ifconfig：	查看网络接口信息
//4.firewall：	设置防火墙
//5.lsfire：	查看防火墙信息
//6.save:		保存防火墙信息到指定文件
//7.exit：		退出，并清空缓存

//防火墙命令协议
//1.add_ip:		屏蔽该ip
//2.dele_ip:	解除该ip的屏蔽
//3.print_ip:	打印ip过滤链表
//4.add_mac:	屏蔽该mac
//5.dele_mac:	解除该mac的屏蔽
//6.print_mac:	打印mac过滤链表

/******************************************************************
功	能:	键盘输入处理线程
参	数:	无
返回值: 无
*******************************************************************/
void *firewall_pthread(void *arg)
{
	//循环获取命令
	while(1)
	{
		int cmd_buf = 0;
		scanf("%d", &cmd_buf);		//从键盘获取命令

		if(cmd_buf == 1)
		{
			printf_help();			//打印帮助信息
		}
		else if(cmd_buf == 2)
		{
			showarp();				//查看arp缓存表
		}
		else if(cmd_buf == 3)
		{
			ifconfig();				//查看网络接口信息
		}
		else if(cmd_buf == 4)
		{
			firewall();				//设置防火墙
		}
		else if(cmd_buf == 5)
		{
			print_firewall();		//查看防火墙信息
		}
		else if(cmd_buf == 6)
		{
			save_firewall_to_file();//保存防火墙信息到指定文件
		}
		else if(cmd_buf == 7)
		{
			tftp_deal();			//上传/下载
		}
		else if(cmd_buf == 8)
		{
			exit_route();			//退出，并清空缓存
		}
		else
		{
			printf("命令无效，请重新输入。\n");
		}
	}
	
	return NULL;
}

/******************************************************************
函数功能:	显示帮助信息
参	  数:	无
返 回 值: 	无
*******************************************************************/
void printf_help()
{
	printf("**************************************\n");
	printf("* 1.    help:  打印帮助信息          *\n");
	printf("* 2. showarp:  打印arp缓存表         *\n");
	printf("* 3.ifconfig:  打印网络接口信息      *\n");
	printf("* 4.firewall:  设置防火墙            *\n");
	printf("* 5.  lsfire:  查看防火墙            *\n");
	printf("* 6.    save:  保存到文件            *\n"); 
	printf("* 7.    tftp:  上传/下载             *\n"); 
	printf("* 8.    exit:  退出程序              *\n"); 
	printf("**************************************\n");
}

/******************************************************************
函数功能:	打印arp缓存表信息
参	  数:	无
返 回 值: 	无
*******************************************************************/
void showarp()
{
	print_link(arp_head);
}

/******************************************************************
函数功能:	显示网卡信息
参	  数:	无
返 回 值:   无
*******************************************************************/
void ifconfig()
{
	int num = get_interface_num();//接口数量
	int i=0;
	printf("网卡信息：\n");
	for(i=0; i<num; i++)
	{
		if(i == 0)
		{
			printf(" %s  ",			net_interface[i].name);
			fflush(stdout);
		}
		else
		{
			printf(" %s",			net_interface[i].name);
			fflush(stdout);
		}
		//打印mac地址
		printf("       mac    : %02x:%02x:%02x:%02x:%02x:%02x\n",
			net_interface[i].mac[0],
			net_interface[i].mac[1],
			net_interface[i].mac[2],
			net_interface[i].mac[3],
			net_interface[i].mac[4],
			net_interface[i].mac[5]
			);
		//打印ip地址
		printf("            ip     : %d.%d.%d.%d\n",
			net_interface[i].ip[0],
			net_interface[i].ip[1],
			net_interface[i].ip[2],
			net_interface[i].ip[3]);
		//打印子网掩码
		printf("            netmask: %d.%d.%d.%d\n",
			net_interface[i].netmask[0],
			net_interface[i].netmask[1],
			net_interface[i].netmask[2],
			net_interface[i].netmask[3]
		);
	}
}

/******************************************************************
函数功能:	设置防火墙
参	  数:	无
返 回 值:   无
*******************************************************************/
void firewall()
{
	//帮助信息
	printf("**************************************\n");
	printf("* 1.  add_ip:	屏蔽该ip             *\n");
	printf("* 2. dele_ip:	解除该ip的屏蔽       *\n");
	printf("* 3.print_ip:	打印ip过滤链表       *\n");
	printf("* 4. add_mac:	屏蔽该mac            *\n");
	printf("* 5.dele_mac:	解除该mac的屏蔽      *\n");
	printf("* 6.prin_mac:	打印mac过滤链表      *\n"); 
	printf("**************************************\n");
	
	int cmd_firewall = 0;
	scanf("%d", &cmd_firewall);//从键盘获取命令
	
	//执行命令
	if(cmd_firewall == 1)
	{
		add_ip();			//屏蔽该ip
	}
	else if(cmd_firewall == 2)
	{
		dele_ip();			//解除该ip的屏蔽
	}
	else if(cmd_firewall == 3)
	{
		print_ip(ip_head);	//打印ip过滤链表
	}
	else if(cmd_firewall == 4)
	{
		add_mac();			//屏蔽该mac
	}
	else if(cmd_firewall == 5)
	{
		dele_mac();			//解除该mac的屏蔽
	}
	else if(cmd_firewall == 6)
	{
		print_mac(ip_head);	//打印mac过滤链表
	}
	else
	{
		printf("命令无效。\n");
	}
}

/******************************************************************
函数功能:	添加ip过滤条件
参	  数:	无
返 回 值:   无
*******************************************************************/
void add_ip()
{
	unsigned char ip_buf[4] = "";
	char temp_buf[100] = "";
	int ip;
	printf("请输入您想要过滤的ip: \n");
	getchar();
	fgets(temp_buf, sizeof(temp_buf), stdin);//获取过滤ip
	temp_buf[strlen(temp_buf)-1]= '\0';		 //转换成字符串
	inet_pton(AF_INET, temp_buf, &ip);		 //把点分十进制数串转换成32位无符号整数
	memcpy(ip_buf, &ip, 4);					 //把ip存入buf
	printf("ip:%d.%d.%d.%d\n", ip_buf[0],ip_buf[1],ip_buf[2],ip_buf[3]);
	
	IP_LINK* p = (IP_LINK*)malloc(sizeof(IP_LINK));	//开辟新节点
	memcpy(p->ip, ip_buf, 4);
	ip_head = insert_ip_link(ip_head, p);			//插入链表
	print_ip(ip_head);						        //打印ip过滤链表
}

/******************************************************************
函数功能:	删除IP过滤条件
参	  数:	无
返 回 值:   无
*******************************************************************/
void dele_ip()
{
	unsigned char ip_buf[4] = "";
	char temp_buf[100] = "";
	int ip;
	printf("请输入您想要删除的ip: ");
	fflush(stdout);
	getchar();
	fgets(temp_buf, sizeof(temp_buf), stdin);//获取过滤ip
	temp_buf[strlen(temp_buf)-1]= '\0';		//转换成字符串
	inet_pton(AF_INET, temp_buf, &ip);		//把点分十进制数串转换成32位无符号整数
	memcpy(ip_buf, &ip, 4);					//把ip存入buf

	ip_head = delete_ip(ip_head, ip_buf);	//删除节点
	print_ip(ip_head);						//打印ip过滤链表
}

/******************************************************************
函数功能:	添加mac过滤条件
参	  数:	无
返 回 值: 	无
*******************************************************************/
void add_mac()
{
	char mac_buf[100] = "";
	char buf[4] = {0,0,0,0};
	printf("请输入您想要过滤的mac: ");
	fflush(stdout);
	getchar();
	fgets(mac_buf, sizeof(mac_buf), stdin);			//获取过滤mac
	mac_buf[strlen(mac_buf)-1]= '\0';	  			//转换成字符串

	IP_LINK* p = (IP_LINK*)malloc(sizeof(IP_LINK));	//开辟新节点
	memcpy(p->mac, mac_buf, strlen(mac_buf));		//拷贝mac信息
	memcpy(p->ip, buf, 4);							//拷贝ip信息
	ip_head = insert_mac_link(ip_head, p);			//插入节点
	print_mac(ip_head);								//打印mac过滤链表
}

/******************************************************************
函数功能:	删除mac过滤条件
参	  数:	无
返 回 值: 	无
*******************************************************************/
void dele_mac()
{
	char mac_buf[100] = "";
	printf("请输入您想要删除的mac: ");
	fflush(stdout);
	getchar();
	fgets(mac_buf, sizeof(mac_buf), stdin);//获取过滤mac
	mac_buf[strlen(mac_buf)-1]= '\0';	   //转换成字符串

	ip_head = delete_mac(ip_head, (unsigned char *)mac_buf);//删除节点
	print_mac(ip_head);						//打印mac过滤链表
}

/******************************************************************
函数功能:	查看防火墙信息
参	  数:	无
返 回 值:	无
*******************************************************************/
void print_firewall()
{
	print_ip(ip_head);	//打印ip过滤链表
	print_mac(ip_head);	//打印mac过滤链表
}

/******************************************************************
函数功能:	保存防火墙信息到文件
参	  数:	无
返 回 值:	无
*******************************************************************/
void save_firewall_to_file()
{
	write_ip_link();//把防火墙信息写入文件
	printf("保存成功！\n");
}

/******************************************************************
函数功能:	退出程序
参    数:	无
返 回 值:   无
*******************************************************************/
void exit_route()
{
	write_ip_link();			//把防火墙信息写入文件
	free_arp_link(arp_head);	//释放arp链表
	free_ip_link(ip_head);		//释放ip链表
	close(sockfd);				//关闭套接字
	_exit(1);					//退出
}

