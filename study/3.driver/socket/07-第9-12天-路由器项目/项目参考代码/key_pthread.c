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
��	��:	void setip(char *msg)
��	��:	���IP��������
��	��:	char *msg �����˵�IP
����ֵ: ��
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
��	��:	void delip(char *msg)
��	��:	ɾ��IP��������
��	��:	char *msg ��ɾ����IP
����ֵ: ��
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
��	��:	void help(char *msg)
��	��:	������Ϣ
��	��:	��
����ֵ: ��
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
��	��:	void showip(char *msg)
��	��:	��ʾIP�����б�
��	��:	��
����ֵ: ��
*******************************************************************/
void showip(char *msg){
	 printf_ip_link(ip_head);
}

/******************************************************************
��	��:	void showarp(char *msg)
��	��:	��ʾARP�б�
��	��:	��
����ֵ: ��
*******************************************************************/
void showarp(char *msg){
	printf_arp_link(arp_head);
}

/******************************************************************
��	��:	void saveset(char *msg)
��	��:	�������еĹ����������浽�����ĵ�
��	��:	��
����ֵ: ��
*******************************************************************/
void saveset(char *msg){
	save_ip_link();
}

/******************************************************************
��	��:	void exit_route(char *msg)
��	��:	�˳�����
��	��:	��
����ֵ: ��
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
��	��:	void *key_pthread(void *arg)
��	��:	�������봦���߳�
��	��:	��
����ֵ: ��
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

