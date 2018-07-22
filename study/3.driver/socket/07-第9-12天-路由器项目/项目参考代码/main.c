#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>//htons
#include <netinet/ether.h>//ETH_P_ALL
#include <pthread.h>
#include "arp_pthread.h"
#include "arp_link.h"
#include "ip_pthread.h"
#include "main.h"
#include "ip_link.h"
#include "key_pthread.h"
#include "get_interface.h"
/******************************************************************
函	数:	int main(int argc, char *argv[])
功	能:	主函数
参	数:	无
返回值: 无
*******************************************************************/
int main(int argc, char *argv[]){
	//初始化 配置文件
	init_ip_link();
	//获取接口信息
	getinterface();
	//创建键盘处理函数并脱离
	pthread_t KEY_T;
	pthread_create(&KEY_T, NULL,key_pthread, NULL);
	pthread_detach(KEY_T);
	
	//创建原始套接字,接收所有类型的数据包
	raw_sock_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(raw_sock_fd<=0){
		perror("socket");
		_exit(-1);
	}
	char recv_buff[RECV_SIZE]="";//原始套接字数据包大约为1500个字节
	ssize_t recv_len=0;
	while(1){
		bzero(recv_buff,sizeof(recv_buff));
		recv_len = recvfrom(raw_sock_fd, recv_buff, sizeof(recv_buff), 0, NULL, NULL);
		if(recv_len<=0||recv_len>RECV_SIZE){
			perror("recvfrom");
			continue;
		}
		if((recv_buff[12]==0x08)&&(recv_buff[13]==0x06)){//ARP协议包
			ARP_LINK *p = (ARP_LINK *)malloc(sizeof(ARP_LINK));
			if(p==NULL){
				perror("malloc");
				continue;
			}
			memcpy(p->mac, recv_buff+22, 6);//mac
			memcpy(p->ip , recv_buff+28, 4);//ip
			//printf("%d.%d.%d.%d-->",p->ip[0],p->ip[1],p->ip[2],p->ip[3]);
			pthread_t ARP_T;
			pthread_create(&ARP_T, NULL,arp_pthread, (void*)p);
			pthread_detach(ARP_T);
		}
		if((recv_buff[12]==0x08)&&(recv_buff[13]==0x00)){//IP协议包
			//目的ip过滤
			IP_LINK *ip_pb = find_ip(ip_head, (unsigned char*)recv_buff+30);
			if(ip_pb!=NULL){
				continue;
			}
			RECV_DATA *recv = (RECV_DATA *)malloc(sizeof(RECV_DATA));
			recv->data_len = recv_len;
			memcpy(recv->data, recv_buff, recv_len);
			//创建转发数据包处理线程
			pthread_t IP_T;
			pthread_create(&IP_T, NULL,ip_pthread, (void*)recv);
			pthread_detach(IP_T);
		}
	}
	
	return 0;
}