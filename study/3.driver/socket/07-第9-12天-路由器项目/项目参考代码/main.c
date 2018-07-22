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
��	��:	int main(int argc, char *argv[])
��	��:	������
��	��:	��
����ֵ: ��
*******************************************************************/
int main(int argc, char *argv[]){
	//��ʼ�� �����ļ�
	init_ip_link();
	//��ȡ�ӿ���Ϣ
	getinterface();
	//�������̴�����������
	pthread_t KEY_T;
	pthread_create(&KEY_T, NULL,key_pthread, NULL);
	pthread_detach(KEY_T);
	
	//����ԭʼ�׽���,�����������͵����ݰ�
	raw_sock_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(raw_sock_fd<=0){
		perror("socket");
		_exit(-1);
	}
	char recv_buff[RECV_SIZE]="";//ԭʼ�׽������ݰ���ԼΪ1500���ֽ�
	ssize_t recv_len=0;
	while(1){
		bzero(recv_buff,sizeof(recv_buff));
		recv_len = recvfrom(raw_sock_fd, recv_buff, sizeof(recv_buff), 0, NULL, NULL);
		if(recv_len<=0||recv_len>RECV_SIZE){
			perror("recvfrom");
			continue;
		}
		if((recv_buff[12]==0x08)&&(recv_buff[13]==0x06)){//ARPЭ���
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
		if((recv_buff[12]==0x08)&&(recv_buff[13]==0x00)){//IPЭ���
			//Ŀ��ip����
			IP_LINK *ip_pb = find_ip(ip_head, (unsigned char*)recv_buff+30);
			if(ip_pb!=NULL){
				continue;
			}
			RECV_DATA *recv = (RECV_DATA *)malloc(sizeof(RECV_DATA));
			recv->data_len = recv_len;
			memcpy(recv->data, recv_buff, recv_len);
			//����ת�����ݰ������߳�
			pthread_t IP_T;
			pthread_create(&IP_T, NULL,ip_pthread, (void*)recv);
			pthread_detach(IP_T);
		}
	}
	
	return 0;
}