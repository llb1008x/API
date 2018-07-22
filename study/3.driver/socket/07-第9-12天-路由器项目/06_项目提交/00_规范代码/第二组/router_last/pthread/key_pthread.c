#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <strings.h>
#include <libnet.h>
#include <unistd.h>         
#include "../pthread/arp_pthread.h"
#include "../source/ip_filter.h"
#include "../source/ip_link.h"
#include "../source/get_interface.h"
#include "../project/types.h"
#include"../pthread/key_pthread.h"


void help_information(void)//系统初始界面
{
	
	PRINT("\n\n              ******************************************\n",SKY);
	PRINT("              *     1:        打印帮助信息             *\n",SKY);
	PRINT("              *     2:        查看路由表               *\n",SKY);
	PRINT("              *     3:        设置防火墙信息           *\n",SKY);
	PRINT("              *     4:        查看防火墙信息           *\n",SKY);
	PRINT("              *     5:        清除防火墙               *\n",SKY);
	PRINT("              *     6:        删除一条防火墙信息       *\n",SKY);
	PRINT("              *     7:        ifconfig打印接口信息     *\n",SKY);
	PRINT("              *     0:        退出系统                 *\n",SKY);
	PRINT("              ******************************************\n",SKY);
}
void firewall_information(void)//防火墙设置界面
{
	PRINT("\n\n              ******************************************\n",YELLOW);
	PRINT("              **************请按照以下格式输入**********\n",YELLOW);
	PRINT("              ******           mac:mac地址        ******\n",YELLOW);
	PRINT("              ******           ip:ip地址          ******\n",YELLOW);
	PRINT("              ******           por:协议类型       ******\n",YELLOW);
	PRINT("              ******           port:端口          ******\n",YELLOW);
	PRINT("              ******         0 返回上一级         ******\n",YELLOW);
	PRINT("              ******************************************\n",YELLOW);
	
}
//线程：控制
void *key_arp_control(void *arg)
{
	
	ROU *router = (ROU *)arg;	
	router->filt_link_head =NULL;
	
	read_firewall(router);
	help_information();//打印提示信息	

	unsigned char str[100] = "";
	while(1)
	{
		bzero(str,sizeof(str));
		fgets(str,sizeof(str),stdin);	
		str[strlen(str)-1] = '\0';
		if(strcmp(str,"1") == 0)//提示信息
		{
			system("clear");
			help_information();
		}
		else if(strcmp(str,"2") == 0)//arp缓存表
		{
			system("clear");
			help_information();	
			link_print(router->router_arp_head);
					
		}
		else if(strcmp(str,"3") == 0)//添加防火墙内容
		{
			system("clear");
			while(1)
			{
				firewall_information();
				unsigned char fire[100] = "";
				fgets(fire,sizeof(fire),stdin);
				fire[strlen(fire)-1] = '\0';
				
				if(strcmp(fire,"0") == 0)//退出
				{
					system("clear");
					help_information();
					break;
				}
				router->filt_link_new = (FILT_LINK *)malloc(sizeof(FILT_LINK));
				sprintf(router->filt_link_new->filt_str,"filt_%s",fire);
				system("clear");
				
				if(filt_link_search(router->filt_link_head, router->filt_link_new->filt_str) == 0)
				{
					filt_link_create(&router->filt_link_head,router->filt_link_new);
					write_firewall(router);
					PRINT("设置成功！\n",GREEN);
				}
				else
				{
					PRINT("已有此条过滤信息:",RED);
					printf("%s\n",router->filt_link_new->filt_str);
				}
			}
		}
		else if(strcmp(str,"4") == 0)
		{
			system("clear");
			help_information();
			filt_link_print(router->filt_link_head);
			
		}
		else if(strcmp(str,"5") == 0)//删除防火墙内容
		{
			system("clear");
			filt_link_free(&router->filt_link_head);
			remove("../filt_file/fire.txt");
			help_information();
		}
		else if(strcmp(str,"6") == 0)//删除一条防火墙信息
		{
			system("clear");
			help_information();
			unsigned char delete[100] = "";
			if(router->filt_link_head != NULL)
			{
				filt_link_print(router->filt_link_head);
				
				fgets(delete,sizeof(delete),stdin);
				delete[strlen(delete)-1] = '\0';
			
			}
			
			if(filt_link_delete(&router->filt_link_head,delete)==1)
			{
				write_firewall(router);
				system("clear");
				
				PRINT("删除结点成功！\n",GREEN);
				help_information();
			}
			else
			help_information();
			
			
		}
		else if(strcmp(str,"7") == 0)//ifconfig
		{
			system("clear");
			help_information();
			getinterface();//接口函数
			 
		}
		else if(strcmp(str,"0") == 0)//退出系统
		{
			system("clear");
			close(router->sockfd);
			exit(-1);
		}
	}	
	return NULL;

	
}