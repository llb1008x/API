#include <stdio.h>
#include <stdlib.h>
#include <string.h>						
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>	

/************************************************************************
函数名称：	void main(int argc, char *argv[])
函数功能：	主函数，用进程建立一个TCP Echo Server
函数参数：	无
函数返回：	无
************************************************************************/
int main(int argc, char *argv[])
{
	unsigned short port = 8000;
	if(argc > 1)
	{
		port = atoi(argv[1]);
	}
	
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
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
		perror("listen");
		close(sockfd);
		exit(-1);
	}
	
	while(1)               //主进程 循环等待客户端的连接
	{
		
		char cli_ip[INET_ADDRSTRLEN] = "";
		struct sockaddr_in client_addr;
		socklen_t cliaddr_len = sizeof(client_addr);
		
		int connfd = accept(sockfd, (struct sockaddr*)&client_addr, &cliaddr_len);
		if(connfd < 0)
		{
			perror("accept");
			close(sockfd);
			exit(-1);
		}
		
		if(fork() == 0)     //子进程 接收客户端的信息，并发还给客户端
		{
			close(sockfd);   //关闭监听套接字
			char recv_buf[1024] = "";
			int recv_len = 0;
			
			inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
			
			while((recv_len = recv(connfd, recv_buf, sizeof(recv_buf), 0)) > 0)
			{
				send(connfd, recv_buf, recv_len, 0);
			}
			
			printf("client closed!\n");
			exit(0);
		}	
		close(connfd);    //关闭已连接套接字
	}
	close(sockfd);
    return 0;
}