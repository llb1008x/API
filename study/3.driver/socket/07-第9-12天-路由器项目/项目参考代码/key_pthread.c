#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "ip_link.h"
#include "arp_link.h"
typedef void (*FUN) (char *);

typedef struct cmd{
	char cmd_str[20];
	FUN fun;
}KEY_CMD;

/******************************************************************
函	数:	void setip(char *msg)
功	能:	添加IP过滤条件
参	数:	char *msg 待过滤的IP
返回值: 无
*******************************************************************/
void setip(char *msg){
	unsigned char msg_ip[4];
	int ip;
	inet_pton(AF_INET, msg, &ip);
	memcpy(msg_ip, &ip, 4);
	printf("%d.%d.%d.%d",msg_ip[0],msg_ip[1],msg_ip[2],msg_ip[3]);
	if((msg_ip[3]==0)||(msg_ip[3]==255)||(msg_ip[0]==0)){
		return;
	}
	IP_LINK* p = (IP_LINK*)malloc(sizeof(IP_LINK));
	memcpy(p->ip, msg_ip, 4);
	ip_head = inner_ip_link(ip_head, p);
	printf_ip_link(ip_head);
}
/******************************************************************
函	数:	void delip(char *msg)
功	能:	删除IP过滤条件
参	数:	char *msg 待删除的IP
返回值: 无
*******************************************************************/
void delip(char *msg){
	unsigned char msg_ip[4];
	int ip;
	inet_pton(AF_INET, msg, &ip);
	memcpy(msg_ip, &ip, 4);
	printf("%d.%d.%d.%d",msg_ip[0],msg_ip[1],msg_ip[2],msg_ip[3]);
	if((msg_ip[3]==0)||(msg_ip[3]==255)||(msg_ip[0]==0)){
		return;
	}
	ip_head = del_ip_for_link(ip_head,msg_ip);
	printf_ip_link(ip_head);
}

/******************************************************************
函	数:	void help(char *msg)
功	能:	帮助信息
参	数:	无
返回值: 无
*******************************************************************/
void help(char *msg){
	printf("=------------------------=\n");
	printf("=---setip:172.20.226.4---=\n");
	printf("=---delip:172.20.226.4---=\n");
	printf("=---saveset--------------=\n");
	printf("=---showip---------------=\n"); 
	printf("=---showarp--------------=\n");
	printf("=------------------------=\n");	 
}

/******************************************************************
函	数:	void showip(char *msg)
功	能:	显示IP过滤列表
参	数:	无
返回值: 无
*******************************************************************/
void showip(char *msg){
	 printf_ip_link(ip_head);
}

/******************************************************************
函	数:	void showarp(char *msg)
功	能:	显示ARP列表
参	数:	无
返回值: 无
*******************************************************************/
void showarp(char *msg){
	printf_arp_link(arp_head);
}

/******************************************************************
函	数:	void saveset(char *msg)
功	能:	将链表中的过滤条件保存到配置文档
参	数:	无
返回值: 无
*******************************************************************/
void saveset(char *msg){
	save_ip_link();
}

/******************************************************************
函	数:	void exit_route(char *msg)
功	能:	退出程序
参	数:	无
返回值: 无
*******************************************************************/
void exit_route(char *msg){
	save_ip_link();
	free_ip_link(ip_head);
	free_arp_link(arp_head);
	_exit(1);
}

KEY_CMD key_cmd[]={
			{"help",help},
			{"setip",setip},
			{"delip",delip},
			{"showip",showip},
			{"showarp",showarp},
			{"saveset",saveset},
			{"exit",exit_route}
			};
/******************************************************************
函	数:	void *key_pthread(void *arg)
功	能:	键盘输入处理线程
参	数:	无
返回值: 无
*******************************************************************/
void *key_pthread(void *arg){
	printf("------------key_pthread------------------\n");
	help("--");
	while(1){
		char buff[100]="";
		char cmd[100]="";
		char msg[100]="";
		fgets(buff,sizeof(buff),stdin);
		buff[strlen(buff)-1]='\0';
		sscanf(buff,"%[^:]:%s",cmd,msg);
		int i;
		for(i=0;i<(sizeof(key_cmd)/sizeof(KEY_CMD));i++){
			if(strcmp(cmd,key_cmd[i].cmd_str)==0){
				key_cmd[i].fun(msg);
				break;
			}
		}
	}
	return NULL;
}

