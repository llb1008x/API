#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "arp_link.h"
#include "ip_link.h"

//帮助信息
void help()
{
	printf("******************************\n");
	printf("**----help---show-help-info-**\n");
	printf("**----setip:10.220.4.250----**\n");
	printf("**----delip:10.220.4.250----**\n");
	printf("**----saveip----------------**\n");
	printf("**----showip----------------**\n"); 
	printf("**----showarp---------------**\n");
	printf("******************************\n");	 
}
//显示arp
void showarp(){
	printf_arp_link(arp_head);
}
//设置过滤ip
void setip()
{
	 set_ip();
}

void showip()
{	
	 printf_ip_link(ip_head);
}

void del()
{
	del_ip_link(ip_head);
}

void saveip()
{
	save_ip_link();
}
typedef void (*FUN) (void );

typedef struct cmd{
	char cmd_str[20];
	FUN fun;
}KEY_CMD;

KEY_CMD key_cmd[]=
{
		{"help",help},
		{"showarp",showarp},
		{"setip",setip},
		{"showip",showip},
		{"del",del},
		{"saveip",saveip}
					
};

void *key_pthread(void *arg)
{
	printf("------------key_pthread-------------\n");
	help();
	while(1)
	{
		char buff[100]="";
		fgets(buff,sizeof(buff),stdin);
		buff[strlen(buff)-1]=0;
		int i;
		for(i=0;i<(sizeof(key_cmd)/sizeof(KEY_CMD));i++)
		{
			if(strcmp(buff,key_cmd[i].cmd_str)==0)
			{
				key_cmd[i].fun();
				
			}
		}
	}
}