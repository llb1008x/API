#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>			
#include <string.h>			
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/************************************************************************
函数名称：	int main(int argc, char *argv[])
函数功能：	主函数，建立一个TCP Echo Client
函数参数：	可以传参改变服务器的ip地址和其端口号，亦可不传参使用默认值
函数返回：	无
************************************************************************/
int main(int argc, char *argv[])
{
	unsigned short port = 8000;
	char *server_ip = "172.20.223.55";
	
	if(argc > 1)
	{
		server_ip = argv[1];
	}
	
	if(argc > 2)
	{
		port = atoi(argv[2]);
	}
	
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}
	
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	inet_pton(AF_INET, server_ip, &server_addr.sin_addr);
	
	int err_log = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(err_log != 0)
	{
		perror("connect");
		close(sockfd);
		exit(-1);
	}
	
	printf("connect to %s success\nplease input your message\n", server_ip);
	
	while(1)
	{
		char send_buf[512] = "";
		char recv_buf[512] = "";
		
		fgets(send_buf, sizeof(send_buf), stdin);
		send_buf[strlen(send_buf)-1] = '\0';
		
		if(strcmp(send_buf, "exit") == 0)
		{
			close(sockfd);
			exit(0);
		}
		
		send(sockfd, send_buf, strlen(send_buf), 0);
		recv(sockfd, recv_buf, sizeof(recv_buf), 0);
		printf("recv message:%s\n", recv_buf);
		
	}
}

