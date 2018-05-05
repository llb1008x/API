#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>			
#include <string.h>			
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

/************************************************************************
函数名称：	void *write_thread(void *arg)
函数功能：	该线程函数只是用来发送TCP消息
函数参数：	void *arg，已连接套接字
函数返回：	void *
************************************************************************/
void *write_thread(void *arg)
{
	int sockfd = (int)arg;
	
	while(1)
	{
		char send_buf[1024] = "";	
		printf("send>:");										//客户端输入信息
					
		fgets(send_buf, sizeof(send_buf), stdin);
		send_buf[strlen(send_buf)-1] = '\0';
		if( strcmp(send_buf, "exit") == 0)						//是否要退出
		{
			printf("\nbyebye\n");
			close(sockfd);
			exit(0);
		}
		send(sockfd, send_buf, strlen(send_buf), 0);						//发送信息
	}
	return NULL;
}

/************************************************************************
函数名称：	int main(int argc, char *argv[])
函数功能：	主函数，建立一个chat_client
函数参数：	可以传参改变服务器的ip地址和其端口号，亦可不传参使用默认值
函数返回：	无
************************************************************************/

int main(int argc , char *argv[])
{
	
	
	unsigned short port = 8000;											//服务器端口号
	char *server_ip = "172.20.223.55";										//服务器IP地址
	

	if( argc > 1 )													//服务器ip地址
	{		
		server_ip = argv[1];
	}	
	if( argc > 2 )													//服务器端口
	{
		port = atoi(argv[2]);
	}
	

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);							//创建套接字
	if( sockfd < 0 )
	{
		perror("socket");
		exit(-1);
	}
	
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	inet_pton(AF_INET, server_ip, &server_addr.sin_addr);
	
	if( connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0)		//连接服务器
	{
		perror("connect");
		close(sockfd);		
		exit(-1);
	}
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, (void *)write_thread, (void *)sockfd);
	
	while(1)
	{
		char recvbuf[1024] = "";
		
		recv(sockfd, recvbuf, sizeof(recvbuf), 0);				//接收客户端信息
		
		printf("\rrecv:%s\n", recvbuf);
		printf("send>:");
		fflush(stdout);
	}
	close(sockfd);
	return 0;
}


