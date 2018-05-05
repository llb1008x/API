#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <net/if.h>				//struct ifreq
#include <sys/ioctl.h>			//ioctl、SIOCGIFADDR
#include <sys/socket.h>
#include <netinet/ether.h>		//ETH_P_ALL
#include <netpacket/packet.h>	//struct sockaddr_ll
#include <netinet/in.h>
#include <arpa/inet.h>
	
void *rec_buffer(void *buf)
{
	unsigned short port = 8080;

	int sockfd;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}
		
	struct sockaddr_in my_addr;
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port   = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	printf("Binding server to port %d\n", port);
	int err_log;
	err_log = bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));
	if(err_log != 0)
	{
		perror("bind");
		close(sockfd);		
		exit(-1);
	}
	printf("receive data...\n");
	while(1)
	{
		int recv_len;
		char recv_buf[512] = "";
		struct sockaddr_in client_addr;
		char cli_ip[INET_ADDRSTRLEN] = "";//INET_ADDRSTRLEN=16
		socklen_t cliaddr_len = sizeof(client_addr);
		
		recv_len = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*)&client_addr, &cliaddr_len);
		inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
		printf("\nip:%s ,port:%d\n",cli_ip, ntohs(client_addr.sin_port));
		printf("data(%d):%s\n",recv_len,recv_buf);

	}
	close(sockfd);
	return 0;
}
	
int main(int argc, char *argv[])
{
	pthread_t p_th;
	unsigned short port = 8080;
	char *server_ip = "172.20.226.1";
	
	if( argc > 1 )									//服务器ip地址
	{	
		server_ip = argv[1];
	}
	
	if( argc > 2 )									//服务器端口
	{
		port = atoi(argv[2]);
	}

	int sockfd;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);    	//创建UDP套接字
	if(sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}
	
	struct sockaddr_in dest_addr;
	bzero(&dest_addr, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port   = htons(port);
	inet_pton(AF_INET, server_ip, &dest_addr.sin_addr);

	printf("send data to UDP server %s:%d!\n", server_ip, port);
	pthread_create(&p_th,NULL,rec_buffer,NULL);
	while(1)
	{//printf("sendzzz\n");
		char send_buf[512] = "";
		int len = sprintf(send_buf, "Hello world");//5个字节
	//	fgets(send_buf, sizeof(send_buf), stdin);//获取输入
		send_buf[strlen(send_buf)] = '\0';
		sendto(sockfd, send_buf, strlen(send_buf)+1, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));//发送数据
		sleep(1);
	}
	
	close(sockfd);
	return 0;
}