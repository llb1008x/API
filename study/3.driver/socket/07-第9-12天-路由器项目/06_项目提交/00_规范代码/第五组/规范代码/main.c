#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ether.h>
#include "get_interface.h"
#include "arp_pthread.h"
#include "arp_link.h"
#include "key_pthread.h"

ARP_LINK *arp_head = NULL;

int main(int argc,char **argv)
{
	getinterface();//获得接口数据

	//键盘处理函数
	pthread_t key_dell;
	pthread_create(&key_dell, NULL,key_pthread, NULL);
	pthread_detach(key_dell);
	
	int raw_sock_fd = socket(PF_PACKET,SOCK_RAW,htons(ETH_P_ALL));
	if(raw_sock_fd<0)
	{
		perror("socket");		
		exit(-1);
	}
	pthread_t pid_p;

	
	pthread_create(&pid_p, NULL,arp_pthread, (void *)raw_sock_fd);

	
	pthread_join(pid_p,NULL);

	return 0;
}
