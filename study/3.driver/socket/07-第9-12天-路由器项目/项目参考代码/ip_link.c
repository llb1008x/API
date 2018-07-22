#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "ip_link.h"

#define ip_config_name "ip_config"

IP_LINK *ip_head=NULL;

/******************************************************************
函	数:	void save_ip_link()
功	能:	保存链表数据到配置文件
参	数:	无
返回值: 无
*******************************************************************/
void save_ip_link(){
	FILE *ip_config = fopen(ip_config_name,"wb+");
	if(ip_config == NULL){
		perror("!!!configure file,in main.c");
		_exit(1);
	}
	char buff[20]="";
	IP_LINK *pb=ip_head;
	while(pb!=NULL){
		sprintf(buff,"%d.%d.%d.%d\n",pb->ip[0],pb->ip[1],pb->ip[2],pb->ip[3]);
		fputs(buff,ip_config);
		pb = pb->next;
	}
	fclose(ip_config);
}


/******************************************************************
函	数:	void init_ip_link()
功	能:	读取配置文件数据到链表
参	数:	无
返回值: 无
*******************************************************************/
void init_ip_link(){
	FILE *ip_config = NULL;
	ip_config = fopen(ip_config_name,"rb+");
	if(ip_config == NULL){
		perror("!!!configure file,in main.c");
		_exit(1);
	}
	while(1){
		char buff[500]="";
		bzero(buff,sizeof(buff));
		int ip;
		if(fgets(buff,sizeof(buff),ip_config)==NULL){
			break;
		}
		buff[strlen(buff)-1]=0;//注意文件中存在\r
		inet_pton(AF_INET, buff, &ip);
		IP_LINK *pb = (IP_LINK *)malloc(sizeof(IP_LINK));	
		memcpy(pb->ip, &ip, 4);
		ip_head = inner_ip_link(ip_head,pb);
	}
	printf_ip_link(ip_head);
	fclose(ip_config);
}

/******************************************************************
函	数:	void init_ip_link()
功	能:	释放ip过滤链表
参	数:	IP_LINK *head ip过滤链表头
返回值: 无
*******************************************************************/
void free_ip_link(IP_LINK *head){
	IP_LINK *pb = head;
	while(head){
		pb = head->next;
		free(head);
		head = pb;
	}
}

/******************************************************************
函	数:	void printf_ip_link(IP_LINK *head)
功	能:	打印ip过滤链表
参	数:	IP_LINK *head ip过滤链表头
返回值: 无
*******************************************************************/
void printf_ip_link(IP_LINK *head){
	printf("\n---------ip_link_start----------\n");
	IP_LINK *pb = head;
	while(pb!=NULL){
		printf("%d.%d.%d.%d\n", pb->ip[0],pb->ip[1],pb->ip[2],pb->ip[3]);
		pb = pb->next;
	}
	printf("---------ip_link_end----------\n\n");
}

/******************************************************************
函	数:	IP_LINK *find_ip(IP_LINK *head, unsigned char *ip)
功	能:	查找ip过滤链表
参	数:	IP_LINK *head ip过滤链表头  unsigned char *ip 待查找IP
返回值: IP_LINK *找到的节点
*******************************************************************/
IP_LINK *find_ip(IP_LINK *head, unsigned char *ip){
	IP_LINK *pb = head;
	while(pb){
		if(memcmp(pb->ip,ip,4)==0){
			break;
		}
		pb = pb->next;
	}
	return pb;
}

/******************************************************************
函	数:	IP_LINK *find_ip(IP_LINK *head, unsigned char *ip)
功	能:	插入ip过滤链表
参	数:	IP_LINK *head ip过滤链表头  IP_LINK* p 待插入节点
返回值: IP_LINK *找到的节点
*******************************************************************/
IP_LINK *inner_ip_link(IP_LINK *head,IP_LINK* p){
	IP_LINK *pb = find_ip(head, p->ip);//查找是否有该记录
	if(pb==NULL){//未查找到，插入链表，直接插入表头方便
		p->next = head;
		head = p;
	}else{
		free(p);
	}
	return head;
}

/******************************************************************
函	数:	IP_LINK *del_ip_for_link(IP_LINK *head, unsigned char *ip)
功	能:	删除ip过滤链表节点
参	数:	IP_LINK *head ip过滤链表头  unsigned char *ip 待删除IP
返回值: IP_LINK *head ip过滤链表头
*******************************************************************/
IP_LINK *del_ip_for_link(IP_LINK *head, unsigned char *ip){
	IP_LINK *pf,*pb;
	pf = pb = head;
	while(pb){
		if(memcmp(pb->ip,ip,4)==0){
			break;
		}
		pf = pb;
		pb = pb->next;
	}
	if(pb!=NULL){
		if(pb==head){
			head = head->next;
		}else{
			pf->next = pb->next;
		}
		free(pb);
		pb = NULL;
	}
	return head;
}

