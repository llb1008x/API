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
函数功能：	线程函数，发送数据
函数参数：	已连接套接字
函数返回：	无
************************************************************************/
void *write_thread(void *arg)
{
	int connfd = (int)arg;
	
	while(1)
	{
		char send_buf[1024] = "";	
		printf("send>:");										//客户端输入信息
			
		fgets(send_buf, sizeof(send_buf), stdin);
		send_buf[strlen(send_buf)-1] = '\0';
		if( strcmp(send_buf, "exit") == 0)						//是否要退出
		{
			printf("\nbyebye\n");
			close(connfd);
			return NULL;
		}
		send(connfd, send_buf, strlen(send_buf), 0);						//发送信息
	}
	return NULL;
}

/************************************************************************
函数名称：	int main(int argc, char *argv[])
函数功能：	主函数，建立一个tcp_qq
函数参数：	可以传参改变自己的端口号，亦可不传参使用默认值
函数返回：	无
************************************************************************/
int main(int argc , char *argv[])
{
	unsigned short port = 8000;								    //服务器默认端口号为8000
	if( argc > 1 )												//设置服务器端口号
	{	
		port = atoi(argv[1]);
	}

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);				//创建TCP Socket
	if( sockfd<0 )
	{
		perror("socket");
		exit(-1);
	}
	
	struct sockaddr_in my_addr;
	bzero(&my_addr,sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port   = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int err_log = bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));
	if(err_log != 0)	//设置TCP Socket地址信息
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
	printf("Listenning at %d\n", port);
	
	while(1)
	{
		struct sockaddr_in client_addr;
		socklen_t cliaddr_len = sizeof(client_addr);
		bzero(&client_addr, sizeof(client_addr));
		
		int connfd = accept(sockfd, (struct sockaddr*)&client_addr, &cliaddr_len);	//等待客户端连接
		if( connfd < 0 )
		{
			perror("accept");
			close(sockfd);
			exit(-1);
		}
		
		//客户端连接成功之后,创建线程
		pthread_t thread_id;
		pthread_create(&thread_id, NULL, (void *)write_thread, (void *)connfd);
		pthread_detach(thread_id);
	
		char cli_ip[INET_ADDRSTRLEN] = "";
		inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
		printf("%s connect you !\n", cli_ip);
		
		//主线程用于接收对方发送来的数据
		while(1)	
		{	
			char recvbuf[200] = "";
			//recv接收到的数据等于0表示对方已断开连接
			if(recv(connfd, recvbuf, sizeof(recvbuf), 0) == 0)				//接收客户端信息
			{	 
				printf("\rclient %s closed\n", cli_ip);
				break;
			}
			printf("\rrecv:%s\n", recvbuf);
			printf("send>:");
			fflush(stdout);
		}
		close(connfd);
	}
	close(sockfd);
	return 0;
}


