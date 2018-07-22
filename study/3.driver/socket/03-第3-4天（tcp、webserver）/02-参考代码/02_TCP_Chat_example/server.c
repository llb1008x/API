#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>			
#include <string.h>			
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/************************************************************************
函数名称：	int main(int argc, char *argv[])
函数功能：	主函数，建立一个TCP Chat Room
函数参数：	可以传参改变自己的端口号，亦可不传参使用默认值
函数返回：	无
************************************************************************/
int main(int argc, char *argv[])
{
	unsigned short port = 8000;
	if(argc >1)
	{
		port = atoi(argv[1]);
	}
	
	int sockfd = socket(AF_INET, SOCK_STREAM,0);
	if(sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}
	
	struct sockaddr_in my_addr;
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int err_log = bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));
	if(err_log != 0)
	{
		perror("bind");
		close(sockfd);
		exit(-1);
	}
	
	err_log = listen(sockfd, 10);
	if(err_log != 0)
	{
		printf("listen");
		close(sockfd);
		exit(-1);
	}
	
	struct sockaddr_in client_addr;
	char cli_ip[INET_ADDRSTRLEN] = "";
	socklen_t cliaddr_len = sizeof(client_addr);
	
	int connfd = accept(sockfd, (struct sockaddr*)&client_addr, &cliaddr_len);
	if(connfd < 0)
	{
		perror("accept");
		close(sockfd);
		exit(-1);
	}
	
	inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
	
	while(1)
	{
		char recv_buf[1024] ="";
		char send_buf[1024] ="";
		
		if(recv(connfd, recv_buf, sizeof(recv_buf), 0) == 0)
		{
			printf("client closed!\n");
			close(sockfd);
			close(connfd);
			exit(-1);
		}
		
		printf("%s : %s\n", cli_ip, recv_buf);
		
		fgets(send_buf, sizeof(send_buf), stdin);
		send_buf[strlen(send_buf)-1] = '\0';
		send(connfd, send_buf, strlen(send_buf), 0);
	}
	
	close(sockfd);
	close(connfd);
	return 0;
}
