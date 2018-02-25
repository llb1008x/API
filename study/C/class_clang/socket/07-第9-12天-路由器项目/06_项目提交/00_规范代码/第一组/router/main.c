#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ether.h>
#include "key_pthread.h"
#include "ping_pthread.h"
#include "get_interface.h"
#include "ip_link.h"
#include "types.h"
#include "main.h"

int main(int argc,char *argv[])
{
	pthread_t tid_ping,tid_key;

	/*读取过滤配置文档信息到链表*/
	read_ip_config();
	
	/*创建原始套接字*/
	sock_raw_fd=socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	if(sock_raw_fd<0)
	{
		perror("socket");
		exit(0);
	}
	
	/*获取本地网络接口*/
	getinterface();
	
	/*创建线程*/
	pthread_create(&tid_ping,NULL,(void *)pthread_ip,NULL);
	pthread_create(&tid_key,NULL,(void *)pthread_key,NULL);
	
	pthread_join(tid_ping,NULL);
	pthread_join(tid_key,NULL);
	
	close(sock_raw_fd);
	return 0;
}